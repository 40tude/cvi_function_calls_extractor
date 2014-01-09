#include <windows.h>  
#ifdef _CVI_         
  #include <utility.h> // SetBreakOnLibraryErrors
#endif
#include <winreg.h>   
#include <objbase.h>
#include <shlobj.h>
#include <stdio.h>
#include <stdlib.h>

#include "GetCVIDir.h"

// ----------------------------------------------------------------------------
static int      VerToInt(char *Buffer);
static HRESULT  ResolveIt(HWND hwnd, LPCTSTR lpszLinkFile, LPTSTR lpszPath, int iPathBufferSize);

// ----------------------------------------------------------------------------
int GetCVI_SamplesDir(char *SamplesDir){
  
  static char CVI_Dir[MAX_PATH];
  
  int err = GetCVIDir(CVI_Dir); 
  if(!err){
    // CVI_Dir already has a '\' at the end of the string
    strcat(CVI_Dir, "samples\\CVI Samples.lnk");
#ifdef _CVI_         
    SetBreakOnLibraryErrors (0); 
#endif
    FILE *tmp = fopen (CVI_Dir, "r");
#ifdef _CVI_         
    SetBreakOnLibraryErrors (1); 
#endif
    if(tmp){
      fclose(tmp);
      err = ResolveIt(NULL, CVI_Dir, SamplesDir, MAX_PATH);
    }else{
      err = -1;
    }
  }
  return err;
}

//-----------------------------------------------------------------------------
// Use the information into the registry to find out the latest version of
// CVI installed on the PC
// No more than MAX_NB_CVI_COPY (10 by default) copy of CVI can be 
// detected on the same PC

// Used to store the differents CVI installations on the PC where the wizard is used
typedef struct tag_CVI_INFO{
  int   Index;
  char  Value[MAX_PATH];
  int   nVer;
}CVI_INFO;  

int GetCVIDir(char *CVIDir){
  
  const char CVIReg[MAX_PATH] = "SOFTWARE\\Wow6432Node\\National Instruments\\CVI";
  HKEY       handle;
  //TODO : manage error code     
  int  error = RegOpenKeyEx (HKEY_LOCAL_MACHINE, CVIReg, 0, KEY_ENUMERATE_SUB_KEYS, &handle);
  
  const int MAX_NB_CVI_COPY = 10;    
  CVI_INFO ArrOfCVIInfo[MAX_NB_CVI_COPY];
  memset(ArrOfCVIInfo, 0, MAX_NB_CVI_COPY*sizeof(CVI_INFO));
  int   i;
  error = ERROR_SUCCESS;
  
  for (i = 0; !error; i++){ 
    if(i == MAX_NB_CVI_COPY) break;
    
    int         BufferSize  = MAX_PATH;
    int         ClassSize   = MAX_PATH;
    int         *Reserved   = NULL;
    FILETIME    LastWrite;  
    static char Buffer[MAX_PATH]; 
    static char Class[MAX_PATH];     
    error = RegEnumKeyEx(handle, i, Buffer, (LPDWORD)&BufferSize, (LPDWORD)Reserved, Class, (LPDWORD)&ClassSize, &LastWrite); 
    if (!error){ 
      ArrOfCVIInfo[i].Index = i;
      strcpy(ArrOfCVIInfo[i].Value, Buffer);
      ArrOfCVIInfo[i].nVer = VerToInt(Buffer);
    } 
  }  
  error = RegCloseKey(handle);   
  
  // Case where we are not able to find a CVI on the current PC
  if(i==0){
    CVIDir="";
    error=-1;
    goto Error;
  }
  
  // At this point we read the array of CVI Info and find out the lastest version of CVI available
  int VerMaxId  = 0;
  int VerMax    = 0;      
  for (int j=0; j != MAX_NB_CVI_COPY; j++){
    if(ArrOfCVIInfo[j].nVer>=VerMax){
      VerMaxId = j;
      VerMax = ArrOfCVIInfo[j].nVer;
    } 
  }
  
  static char Buffer2[MAX_PATH];      
  sprintf(Buffer2, "%s\\%s", CVIReg, ArrOfCVIInfo[VerMaxId].Value);
  error = RegOpenKeyEx (HKEY_LOCAL_MACHINE, Buffer2, 0, KEY_QUERY_VALUE, &handle);
  
  static char Value[MAX_PATH];  
  int         type = REG_SZ;   
  int         psize = MAX_PATH-1;              
  error = RegQueryValueEx (handle, "InstallDir", NULL, (LPDWORD)&type, (LPBYTE)Value, (LPDWORD)&psize);
  error = RegCloseKey (handle);
  strcpy(CVIDir, Value);

Error:
  return(error);
}

//-----------------------------------------------------------------------------
// Convert 4.0.1 into 401 or 5.0.0 in 500
static int VerToInt(char *Buffer){
  
  static char Buffer2[MAX_PATH];               
  memset (Buffer2, 0, MAX_PATH*sizeof(char));
  char *ptr = Buffer;
  int lenght = (int)strlen(Buffer); // TODO : Win64
  for(int i=0, j=0; i!=lenght; i++, ptr++){
    if(*ptr!='.') Buffer2[j++] = *ptr;
  }
  return atoi(Buffer2);
}

// ----------------------------------------------------------------------------
// see http://www.cplusplus.com/forum/windows/64088/
HRESULT ResolveIt(HWND hwnd, LPCTSTR lpszLinkFile, LPTSTR lpszPath, int iPathBufferSize) {

  HRESULT hres = CoInitialize(NULL);
  
  if (SUCCEEDED(hres)) {  
    if (lpszPath == NULL) return E_INVALIDARG;

    *lpszPath = 0;

    // Get a pointer to the IShellLink interface. 
    IShellLink *psl = NULL;
    hres = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLink, (LPVOID *)&psl);
    if (SUCCEEDED(hres)) {
      // Get a pointer to the IPersistFile interface.
      IPersistFile *ppf = NULL;
      hres = psl->lpVtbl->QueryInterface(psl, &IID_IPersistFile, (void **)&ppf);
    
      if (SUCCEEDED(hres)) {
        // Load the shortcut.
        WCHAR wsz[MAX_PATH] = {0};
        // Ensure that the string is Unicode.
        MultiByteToWideChar(CP_ACP, 0, lpszLinkFile, -1, wsz, MAX_PATH);
        hres = ppf->lpVtbl->Load(ppf, wsz, STGM_READ);

        if (SUCCEEDED(hres)) {
          // Resolve the link.
          hres = psl->lpVtbl->Resolve(psl, hwnd, 0);

          if (SUCCEEDED(hres)) {
            // Get the path to the link target.
            hres = psl->lpVtbl->GetPath(psl, lpszPath, MAX_PATH, NULL, SLGP_RAWPATH);        // SLGP_RAWPATH  SLGP_SHORTPATH
          }
        }
        // Release the pointer to the IPersistFile interface.
        ppf->lpVtbl->Release(ppf);
      }
      // Release the pointer to the IShellLink interface.
      psl->lpVtbl->Release(psl);
    }
    CoUninitialize(); 
  }
  return hres;
}
