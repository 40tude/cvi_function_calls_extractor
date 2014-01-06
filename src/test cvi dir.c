#include <windows.h>
#include <winreg.h>

#include <objbase.h>
#include <shlobj.h>
//#include <strsafe.h>

#include <stdio.h>
#include <stdlib.h>

#define MAX_NB_CVI_COPY 10

// ----------------------------------------------------------------------------
static int GetCVIDir(char *CVIDir);
static int VerToInt(char *Buffer);
static HRESULT ResolveIt(HWND hwnd, LPCTSTR lpszLinkFile, LPTSTR lpszPath, int iPathBufferSize);

// Used to qualify the differents CVI installation on the PC where the wizard is used
typedef struct tag_CVI_INFO {
  int   Index;
  char  Value[MAX_PATH];
  int   nVer;
} CVI_INFO;


// ----------------------------------------------------------------------------
int main() {

  char CVI_Dir[MAX_PATH];
  GetCVIDir(CVI_Dir);
  printf("%s\n", CVI_Dir);


  const char cachLinkFile[] = "C:\\Program Files (x86)\\National Instruments\\CVI2013\\samples\\CVI Samples.lnk";
  char achPath[MAX_PATH] = {0};
  HRESULT hres = CoInitialize(NULL);
  if(SUCCEEDED(hres)){
    ResolveIt(NULL, cachLinkFile, achPath, MAX_PATH);
    printf("%s\n", achPath);    
    CoUninitialize();
  }



  getchar();
}

//-----------------------------------------------------------------------------
// Use the information into the registry to find out the latest version of
// CVI installed on the PC
// No more than MAX_NB_CVI_COPY (10 by default) copy of CVI can be
// detected on the same PC
static int GetCVIDir(char *CVIDir) {

  int         error=0;
  int         i, status;
  LONG        retCode;
  int         BufferSize, ClassSize;
  HKEY        handle;
  int         *Reserved=NULL;
  FILETIME    LastWrite;
  CVI_INFO    ArrOfCVIInfo[MAX_NB_CVI_COPY];

  int         VerMaxId=0, VerMax=0;
  int         type = REG_SZ;
  static char Value[MAX_PATH];
  int         psize = MAX_PATH-1;

  static char Buffer[MAX_PATH];
  static char Class[MAX_PATH];
  static char CVIReg[MAX_PATH] = "SOFTWARE\\Wow6432Node\\National Instruments\\CVI";


  memset(ArrOfCVIInfo, 0, MAX_NB_CVI_COPY*sizeof(CVI_INFO));

  status = RegOpenKeyEx (HKEY_LOCAL_MACHINE, CVIReg, 0, KEY_ENUMERATE_SUB_KEYS, &handle);

  for (i = 0, retCode = ERROR_SUCCESS; retCode == ERROR_SUCCESS; i++) {
    if(i==MAX_NB_CVI_COPY)
      break;
    BufferSize = MAX_PATH;
    ClassSize =  MAX_PATH;
    retCode = RegEnumKeyEx(handle, i, Buffer, (LPDWORD)&BufferSize, (LPDWORD)Reserved, Class, (LPDWORD)&ClassSize, &LastWrite);
    if (retCode == (DWORD)ERROR_SUCCESS) {
      //printf ("Value:%s\n", Buffer);
      ArrOfCVIInfo[i].Index = i;
      strcpy(ArrOfCVIInfo[i].Value, Buffer);
      ArrOfCVIInfo[i].nVer = VerToInt(Buffer);
    }
  }
  status = RegCloseKey(handle);

  // Case where we can't find a CVI on the current PC
  if(i==0) {
    CVIDir="";
    error=-1;
    goto Error;
  }

  // At this point we read the array of CVI Info and find out the lastest version of CVI available
  VerMaxId = 0;
  for (i=0; i<MAX_NB_CVI_COPY; i++) {
    if(ArrOfCVIInfo[i].nVer>=VerMax) {
      VerMaxId = i;
      VerMax = ArrOfCVIInfo[i].nVer;
    }
  }

  sprintf(Buffer, "%s\\%s", CVIReg, ArrOfCVIInfo[VerMaxId].Value);
  status = RegOpenKeyEx (HKEY_LOCAL_MACHINE, Buffer, 0, KEY_QUERY_VALUE, &handle);
  status = RegQueryValueEx (handle, "InstallDir", NULL, (LPDWORD)&type, (LPBYTE)Value, (LPDWORD)&psize);
  status = RegCloseKey (handle);

  strcpy(CVIDir, Value);

Error:
  return(error);
}


//-----------------------------------------------------------------------------
// Convert 4.0.1 into 401 or 5.0.0 in 500
static int VerToInt(char *Buffer) {

  char  *ptr=NULL;
  int   i=0, j=0, lenght;

  static char Buffer2[MAX_PATH];

  memset (Buffer2, 0, MAX_PATH*sizeof(char));
  ptr = Buffer;
  // TODO : Win64
  lenght = (int)strlen(Buffer);
  for(i=0, j=0; i<lenght; i++, ptr++) {
    if(*ptr!='.') {
      Buffer2[j++]=*ptr;
    }
  }
  return atoi(Buffer2);
}


// ----------------------------------------------------------------------------
// see http://www.cplusplus.com/forum/windows/64088/
static HRESULT ResolveIt(HWND hwnd, LPCTSTR lpszLinkFile, LPTSTR lpszPath, int iPathBufferSize) {
  
  if (lpszPath == NULL)
    return E_INVALIDARG;

  *lpszPath = 0;

  // Get a pointer to the IShellLink interface. It is assumed that CoInitialize has already been called.
  IShellLink *psl = NULL;
  HRESULT hres = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLink, (LPVOID *)&psl);
  if (SUCCEEDED(hres)) {
    // Get a pointer to the IPersistFile interface.
    IPersistFile *ppf = NULL;
    hres = psl->lpVtbl->QueryInterface(psl, &IID_IPersistFile, (void **)&ppf);
    
    if (SUCCEEDED(hres)) {
      // Add code here to check return value from MultiByteWideChar
      // for success.

      // Load the shortcut.
#ifdef _UNICODE
      hres = ppf->Load(lpszLinkFile, STGM_READ);
#else
      WCHAR wsz[MAX_PATH] = {0};
      // Ensure that the string is Unicode.
      MultiByteToWideChar(CP_ACP, 0, lpszLinkFile, -1, wsz, MAX_PATH);
      hres = ppf->lpVtbl->Load(ppf, wsz, STGM_READ);
#endif

      if (SUCCEEDED(hres)) {
        // Resolve the link.
        hres = psl->lpVtbl->Resolve(psl, hwnd, 0);

        if (SUCCEEDED(hres)) {
          // Get the path to the link target.
          TCHAR szGotPath[MAX_PATH] = {0};
          //hres = psl->lpVtbl->GetPath(psl, szGotPath, _countof(szGotPath), NULL, SLGP_SHORTPATH);
          hres = psl->lpVtbl->GetPath(psl, szGotPath, MAX_PATH, NULL, SLGP_RAWPATH);        // SLGP_RAWPATH  SLGP_SHORTPATH

          if (SUCCEEDED(hres)) {
            //hres = StringCbCopy(lpszPath, iPathBufferSize, szGotPath);
            strcpy(lpszPath, szGotPath); 
          }
        }
      }

      // Release the pointer to the IPersistFile interface.
      ppf->lpVtbl->Release(ppf);
    }

    // Release the pointer to the IShellLink interface.
    psl->lpVtbl->Release(psl);
  }
  return hres;
}
