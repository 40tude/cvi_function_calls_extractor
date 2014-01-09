#include <windows.h>
#include <omp.h>
#include <userint.h>
#include "toolbox.h"
#include <utility.h>
#include <stdio.h>

#include "../assets/frontend.h"
#include "HeaderFooter_HTML.h"
#include "GetCVIDir.h"
#include "clang directives.h"

// ----------------------------------------------------------------------------
static int            ghPanel;        
static int            gFound=0;
static int            gNotCompiled=0;
static HashTableType  ghHash;      

// ----------------------------------------------------------------------------
static void Extract (void);
static void BuildFilesList(const char RootDir[], ListType List);
static void RemoveBlackListed(ListType hList);
static void CollectFunctionCalls(const char *File2Process);
static void SaveHashTable(void);
static void AssemblyCodeAnalysis(const char *File2Process, const char *TempFileName);
static void BodyLoop(const char *RootDir);

// ----------------------------------------------------------------------------
int main (int argc, char *argv[]) {
  
  // TESTING : the next call shows how to make a single file analysis
  //CollectFunctionCalls("C:/Users/Public/Documents/National Instruments/CVI2013/samples/realtime/FileIORT/FileIORT.c");          

  if ((ghPanel = LoadPanel (0, "../assets/frontend.uir", PANEL)) < 0) return -1;
  DisplayPanel (ghPanel);
  Extract();
  RunUserInterface ();
  DiscardPanel (ghPanel);
  return 0;
}

// ----------------------------------------------------------------------------
int CVICALLBACK OnQuit (int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
  
  switch (event) {
    case EVENT_COMMIT:
      QuitUserInterface (0);
    break;
  }
  return 0;
}

// ----------------------------------------------------------------------------
static void Extract (void) {
  
  // DONE : Find out the CVI Samples directory in a more robust way
  char CVI_SamplesDir[MAX_PATH];   
  GetCVI_SamplesDir(CVI_SamplesDir);
  
  // TESTING : analysis directory is a first good candidate
  // Uncomment, comment the lines below to make test
  // Use "" to run the extractor on the complete Samples directory
  const char kStartSubDir[MAX_PATHNAME_LEN]= "\\analysis";
  //const char kStartSubDir[MAX_PATHNAME_LEN]= "";
  //const char kStartSubDir[MAX_PATHNAME_LEN]= "\\dotnet";
  
  char RootDir[MAX_PATHNAME_LEN]="";
  sprintf(RootDir, "%s%s", CVI_SamplesDir, kStartSubDir);
  
  // TESTING : If you want to analyze another directory than the default CVI samples directory - Uncomment the 2 lines below
  // and fill StartSubDirFake with your own path
  //const char StartSubDirFake[MAX_PATHNAME_LEN]= "C:\\Program Files (x86)\\National Instruments\\CVI2013\\toolslib";
  //strcpy(RootDir, StartSubDirFake);  
  
  static char stmp[1024];    
  sprintf(stmp, "Starting directory : %s\n", RootDir);
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp); 
  
  double t1 = Timer ();
    BodyLoop(RootDir);
  double t2 = Timer ();    

  sprintf(stmp, "\nA total of %d .c files was found.", gFound);
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp);    

  sprintf(stmp, "%d of them were NOT analyzed due to compilation errors.", gNotCompiled);
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp); 
  
  SetCtrlVal(ghPanel, PANEL_TIME, t2-t1);
}

// ----------------------------------------------------------------------------
static void BodyLoop(const char *RootDir){
  
  ListType hList = ListCreate (MAX_PATHNAME_LEN); 
  BuildFilesList(RootDir, hList);

  // DONE : find a way to sprintf size_t under WIN32 and WIN64
  static char stmp[1024]; 
  sprintf(stmp, "Initially, %lld .c source files to analyze.", (long long) ListNumItems (hList)); // cast to long long to make sure it work in _WIN32 _WIN64
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp); 

  RemoveBlackListed(hList);

  // DONE : Find a way to support and sprintf size_t correctly under WIN64. Without a cast to int.
  sprintf(stmp, "After removing the black listed files, still %lld .c source files to analyze\n", (long long)ListNumItems (hList)); // cast to long long to make sure it work in _WIN32 _WIN64    
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp); 

  int NbFiles = (int)ListNumItems (hList);

  HashTableCreate (4096, C_STRING_KEY, 0, sizeof(int), &ghHash);        

  // Before to enter the lopp, make sur the user interface is updated
  ProcessDrawEvents();    

  // Uncheck "OpenMP support" in build options if needed
  // Find and check how "#pragma omp critical " is used later in the code
  #pragma omp parallel for default(none) shared(hList, NbFiles, ghHash)
  for(int i=0; i<NbFiles; ++i){
    char File2Process[MAX_PATHNAME_LEN];  
    ListRemoveItem (hList, File2Process, FRONT_OF_LIST);
    CollectFunctionCalls(File2Process); 
  }

  ListDispose (hList); 
  SaveHashTable();    
  HashTableDispose (ghHash);  
}

// ----------------------------------------------------------------------------
/*
Thoses are some of the errors I had and some of the explainations
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/activex/ie/iedemo.c                                      #error  Because of restrictions imposed by IE, this program can only run with administrator privileges
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/apps/spectrum/spectrum.c                                 fatal error: 'dataacq.h' file not found
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/dotnet/FileSystemWatcher/FileSystemWatcher.c             fatal error: 'SystemWrapper.h' file not found. In fact we should have ../SystemWrapper.h
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/dotnet/GenericList/genericlist.c                         I had to had my %USER% .... CVI2013\\samples\\dotnet in the path for included files. See CollectFunctionCalls() 
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/dotnet/Hashtable/hashtable.c                             Idem
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/dotnet/RegEx/DotNetRegularExpressions.c                  Idem
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/dotnet/SimpleTaskMonitor/SimpleTaskMonitor.c             Idem
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/dotnet/Timer/timer.c                                     Idem
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/dotnet/WebService/GetInfoByZip.c                         Idem
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/realtime/FileIORT/FileIORT.c                             _LINK_CVI_LVRT_ must be defined otherwise error: implicit declaration of function 'SleepUS' is invalid in C99.
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/realtime/LoopTimingRT/main_rt.c                          Idem 
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/realtime/NetworkVariableRT/NetworkVariableRT.c           Idem 
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/realtime/RS-232RT/usesDebugViaCom.c                      Idem 
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/realtime/SymmetricMultiprocessingRT/main_rt.c            Idem 
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/realtime/ThreadPriorityRT/main_rt.c                      Idem 
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/realtime/WatchdogRT/main_rt.c                            Idem 
Error while compiling : C:/Users/Public/Documents/National Instruments/CVI2013/samples/userint/custctrl/legend/cklegend.c                       // libprot.h not found
*/

static void RemoveBlackListed(ListType List){

  char *Blacklist[]={                                   \
"\\activex\\ie\\iedemo.c",                              \                                    
"\\apps\\spectrum\\spectrum.c",                         \                                 
"\\dotnet\\FileSystemWatcher\\FileSystemWatcher.c",     \             
"\\realtime\\FileIORT\\FileIORT.c",                     \                             
"\\realtime\\LoopTimingRT\\main_rt.c",                  \                          
"\\realtime\\NetworkVariableRT\\NetworkVariableRT.c",   \           
"\\realtime\\RS-232RT\\usesDebugViaCom.c",              \                      
"\\realtime\\SymmetricMultiprocessingRT\\main_rt.c",    \            
"\\realtime\\ThreadPriorityRT\\main_rt.c",              \                      
"\\realtime\\WatchdogRT\\main_rt.c",                    \                            
"\\userint\\custctrl\\legend\\cklegend.c",              \  
"\\dotnet\\mscorlib2.c",                                \
"\\dotnet\\mscorlib.c",                                 \
};

  int NbFiles = (int)ListNumItems (List); //(int) because of _WIN64
  
  for(int i=0; i!=NbFiles; ++i){
    char File2Process[MAX_PATHNAME_LEN];  
    ListGetItem (List, File2Process, i+1);
    for(int j=0; j!=sizeof(Blacklist)/sizeof(Blacklist[0]); ++j){
      if(*Blacklist[j] && strstr (File2Process, Blacklist[j])){ 
        ListRemoveItem (List, 0, i+1);
        // Since we deleted the element pointed to by i we must take it into account with --i 
        i--;
        // DONE : once found, we remove Blacklist[j] from the array Blacklist to speed up things a little bit
        // see the "*Blacklist[j] &&..." if the if statement
        *Blacklist[j]='\0';
        break;
      }
    }
  }
}

// ----------------------------------------------------------------------------
// SDK FindFirstFile() calls allow recursive directory walking. This is not the case with CVI GetFirstFile() 
static void BuildFilesList(const char RootDir[], ListType List) {
  
  char DirPattern[MAX_PATHNAME_LEN];           
  sprintf(DirPattern,"%s\\*", RootDir);

  WIN32_FIND_DATA ffd;
  HANDLE hFind = FindFirstFile(DirPattern, &ffd);   
  if (hFind!=INVALID_HANDLE_VALUE) { 
    do{
      if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
        if (ffd.cFileName[0] == '.') continue;
        char NewRootDir[MAX_PATHNAME_LEN];             
        sprintf(NewRootDir,"%s\\%s", RootDir, ffd.cFileName);   
        BuildFilesList(NewRootDir, List);
      }
    }while(FindNextFile(hFind, &ffd));
    FindClose(hFind); 
  }
  
  char FilePattern[MAX_PATHNAME_LEN];      
  sprintf(FilePattern,"%s\\*.c", RootDir);
  
  hFind = FindFirstFile(FilePattern, &ffd);
  if (hFind!=INVALID_HANDLE_VALUE) {
    do{
      char File2Process[MAX_PATHNAME_LEN];
      sprintf(File2Process, "%s\\%s", RootDir, ffd.cFileName);
      ListInsertItem (List, File2Process, END_OF_LIST);     
      gFound++;  
    } while (FindNextFile(hFind, &ffd));
  }
  FindClose(hFind);
}

// ----------------------------------------------------------------------------
// TODO : try light options with gCCOptions, gClangDefine and gClangIncludes  
// Called in a thread
static void CollectFunctionCalls(const char *File2Process){ 
  
  // TODO : Find a way to get a safe access to clang.exe. Find out the CVI directory
  const char kPath2Clang[]    = "C:\\Program Files (x86)\\National Instruments\\CVI2013\\bin\\clang\\2.9\\clang.exe";    
  const char kClangOptions[]  = "-cc1 -S -o"; 
  char  TempFileName[L_tmpnam];      
  tmpnam(TempFileName);
  
  static char stmp[1024];  
  static int index=0;
  index++;
  DebugPrintf("%03d - Processing : %s", index, File2Process);       
  sprintf(stmp, "%03d - Processing : %s", index, File2Process);  
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp);  
  ProcessDrawEvents();
  DebugPrintf("%03d Processing : %s", index, File2Process);   
  
  // TESTING : Comment-uncomment Option 1 or Option 2 according to the test you may want to run. 
  // Option 1 should be use in final/release mode.
  // Option 2 is usefull in debug mode.
  /**/
  // Option 1
  char command[10*1024];  
  // see cmd /? - /C execute then ends - /K execute then stay active  
  sprintf(command, "cmd.exe /C  \"    \"%s\" %s \"%s\" %s %s  %s  \"%s\"  \"   ", kPath2Clang, kClangOptions, TempFileName, gkCCOptions, gkClangDefine, gkClangIncludes, File2Process);
  int hExec;       
  // LE_SHOWNORMAL or LE_HIDE
  if(LaunchExecutableEx (command, LE_HIDE, &hExec)>=0) {
  /**/
  
  /*
  // Option 2 
  char command[10*1024];                                  
  // see cmd /? - /C execute then ends - /K execute then stay active
  // see compile.txt for errors and warnings
  sprintf(command, "cmd.exe /K  \"    \"%s\" %s \"%s\" %s %s  %s  \"%s\"  2>compile.txt\"   ", kPath2Clang, kClangOptions, TempFileName, CCOptions, ClangDefine, ClangIncludes, File2Process);
  
  int hExec;       
  // LE_SHOWNORMAL or LE_HIDE
  if(LaunchExecutableEx (command, LE_SHOWNORMAL, &hExec)>=0) {
  */
  
    while(!ExecutableHasTerminated (hExec)) {
      ;
    }
    RetireExecutableHandle(hExec);
  }
  AssemblyCodeAnalysis(File2Process, TempFileName);
}

// ----------------------------------------------------------------------------
static void AssemblyCodeAnalysis(const char *File2Process, const char *TempFileName){

  // assembly code has been generated without error
  if(FileExists (TempFileName, 0)){
    // Get file size
    FILE *hFile = fopen (TempFileName, "rb");
    fseek (hFile, 0, SEEK_END);
    int Size = ftell (hFile);
    rewind (hFile);

    // Load assembly file in memory
    char *Buffer = (char*)malloc((1+Size)*sizeof(char));
    fread (Buffer, sizeof(char), Size, hFile);
    fclose(hFile);
    Buffer[Size]='\0';
  
    // Insert function names in hash table
    char *ptrBegin=NULL;
    char *pBuffer = Buffer;
    while((ptrBegin = strstr (pBuffer, "\tcalll\t_"))){
      ptrBegin = ptrBegin+strlen("\tcalll\t_");
      // 0x0D 0x0A - CR LF - \r\n
      char *ptrEnd = strstr (ptrBegin, "\r\n");
      // Make sure the function's name ends with \0 and insert it in the hash table
      *ptrEnd = '\0';
    
      #pragma omp critical     
      {
        int CurrVal;
        HashTableGetItem (ghHash, ptrBegin, &CurrVal, sizeof(int));
        CurrVal++;
        HashTableInsertItem(ghHash, ptrBegin, &CurrVal); 
      }
      pBuffer = ptrEnd+1;     
    }

    free(Buffer);
    remove (TempFileName);                 
    //*TempFileName='\0';
  }else{
    static char stmp[1024];  
    sprintf(stmp, " *** Error while compiling : \n%s", File2Process);  
    InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp);    
    gNotCompiled++;
  }
}

// ----------------------------------------------------------------------------
// TODO : order the list before generate report
static void SaveHashTable(void){
 
  char ProjectDirName[MAX_PATHNAME_LEN];
  GetProjectDir (ProjectDirName);

  char        PathName[MAX_PATHNAME_LEN];
  const char  kOutputFileName[] = "CrossTable.txt";
  sprintf(PathName, "%s\\%s", ProjectDirName, kOutputFileName);
  
  char        PathName_HTML[MAX_PATHNAME_LEN];
  const char  kOutputFileName_HTML[] = "CrossTable.html";
  sprintf(PathName_HTML, "%s\\%s", ProjectDirName, kOutputFileName_HTML);
  
  FILE *hFile      = fopen (PathName, "w");
  FILE *hFile_HTML = fopen (PathName_HTML, "w");
  
  fprintf(hFile_HTML, "%s", Header_HTML); 
  fprintf(hFile_HTML, "%s", Body1_HTML); 
  fprintf(hFile_HTML, "<p>%s %s</p>\n", "Sample paragraph, blablabla...", DateStr ());   
  fprintf(hFile_HTML, "%s", Body2_HTML);  
  
  HashTableIterator iter;
  for (int status = HashTableIteratorCreate(ghHash, &iter); status >= 0 && status != HASH_TABLE_END; status = HashTableIteratorAdvance(ghHash, iter)) {
    int value;
    // DONE : we should not restrict the lenght of the key (length of the name of the function)        
    char *key=NULL;
    int len = HashTableIteratorGetItem(ghHash, iter, key, 0, &value, sizeof(int));
    key = (char*)malloc(len*sizeof(char));
    status = HashTableIteratorGetItem(ghHash, iter, key, len, &value, sizeof(int));   
    fprintf(hFile, "%s, %d\n", key, value);
    fprintf(hFile_HTML, "<tr><td class=\"Cell_Small\">%s</td>\n<td class=\"Cell_Small\">%d</td></tr>\n", key, value); 
    free(key);
  }
  HashTableIteratorDispose(ghHash, iter);
  fclose(hFile);
  fprintf(hFile_HTML, "%s", Footer_HTML);  
  fclose(hFile_HTML);   
  
  static static char stmp[1024];  
  sprintf(stmp, "\nGenerated report file : ./%s", kOutputFileName);  
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp);
  sprintf(stmp, "This file is comma separated and can be easily imported in MS Excel");  
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp); 
  
  sprintf(stmp, "\nGenerated report file : ./%s", kOutputFileName_HTML);  
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp);
  sprintf(stmp, "This file is HTML5 compliant and can be opened with any browser");  
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp); 
}
