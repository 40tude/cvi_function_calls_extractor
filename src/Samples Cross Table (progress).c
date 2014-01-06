/*
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


#include <windows.h>
#include <omp.h>
#include <userint.h>
#include "toolbox.h"
#include <utility.h>
#include <stdio.h>

#include "frontend.h"
#include "HeaderFooter_HTML.h"
#include "GetCVIDir.h"

//#define WITH_PROGRESS

// ----------------------------------------------------------------------------
static int            ghPanel;        
static int            gFound=0;
static int            gNotCompiled=0;
static HashTableType  ghHash;      

// ----------------------------------------------------------------------------
static void Workload (void);
static void BuildFilesList(const char RootDir[], ListType List);
static void RemoveBlackListed(ListType hList);
static void CollectFunctionCalls(const char *File2Process);
static void SaveHashTable(void);

// ----------------------------------------------------------------------------
int main (int argc, char *argv[]) {

  
  
  if ((ghPanel = LoadPanel (0, "frontend.uir", PANEL)) < 0) return -1;
  DisplayPanel (ghPanel);
  Workload();
  //CollectFunctionCalls("C:/Users/Public/Documents/National Instruments/CVI2013/samples/realtime/FileIORT/FileIORT.c");          
  
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
static void Workload (void) {
  
  HashTableCreate (4096, C_STRING_KEY, 0, sizeof(int), &ghHash);
  
  // DONE : Find out the CVI Samples directory in a more robust way
  char CVI_SamplesDir[MAX_PATH];   
  GetCVI_SamplesDir(CVI_SamplesDir);
  
  //const char StartSubDir[MAX_PATHNAME_LEN]= "\\analysis";
  const char StartSubDir[MAX_PATHNAME_LEN]= "";
  //const char StartSubDir[MAX_PATHNAME_LEN]= "\\dotnet";
  char RootDir[MAX_PATHNAME_LEN]="";
  sprintf(RootDir, "%s%s", CVI_SamplesDir, StartSubDir);
  
  // For testing - If one want to analyze an other directory than the samples directory - Uncomment the 2 line below
  //const char StartSubDirFake[MAX_PATHNAME_LEN]= "C:\\Program Files (x86)\\National Instruments\\CVI2013\\toolslib";
  //strcpy(RootDir, StartSubDirFake);  
  
  char stmp[1024];      
  sprintf(stmp, "Starting directory : %s\n", RootDir);
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp); 
  
#if defined(WITH_PROGRESS)
  SetCtrlAttribute(ghPanel, PANEL_PROGRESSBAR, ATTR_VISIBLE, 1);
#endif 
  ProcessDrawEvents();  
  
  double t1 = Timer ();
    ListType hList = ListCreate (MAX_PATHNAME_LEN); 
    
    BuildFilesList(RootDir, hList);
    
    // DONE : find a way to sprintf size_t under WIN32 and WIN64
#if defined (_WIN64)   
    sprintf(stmp, "Initially, %lld .c source files to analyze.", ListNumItems (hList));
#else
    sprintf(stmp, "Initially, %d .c source files to analyze.", ListNumItems (hList));
#endif
    InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp); 
    ProcessDrawEvents();
    
    RemoveBlackListed(hList);
    // DONE : Find a way to support and sprintf size_t correctly under WIN64. Without a cast to int.
#if defined (_WIN64)       
    sprintf(stmp, "After removing the black listed files, still %lld .c source files to analyze\n", ListNumItems (hList));
#else
    sprintf(stmp, "After removing the black listed files, still %d .c source files to analyze\n", (int)ListNumItems (hList));
#endif
    InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp); 

#if defined(WITH_PROGRESS)
    SetCtrlAttribute(ghPanel, PANEL_PROGRESSBAR, ATTR_MAX_VALUE, (int)ListNumItems (hList));
#endif
    ProcessDrawEvents();

    int NbFiles = (int)ListNumItems (hList);

    // Uncheck "OpenMP support" in build options if needed
    // see the #pragma omp critical 
#if defined(WITH_PROGRESS)         
    int Val=0;
#pragma omp parallel for default(none) shared(hList, NbFiles, ghHash, ghPanel, Val)
#else
#pragma omp parallel for default(none) shared(hList, NbFiles, ghHash)
#endif
    for(int i=0; i<NbFiles; ++i){
      char File2Process[MAX_PATHNAME_LEN];  
      ListRemoveItem (hList, File2Process, FRONT_OF_LIST);
      CollectFunctionCalls(File2Process); 

#if defined(WITH_PROGRESS)      
      GetCtrlAttribute (ghPanel, PANEL_PROGRESSBAR, ATTR_CTRL_VAL, &Val);
      Val++;
      SetCtrlAttribute (ghPanel, PANEL_PROGRESSBAR, ATTR_CTRL_VAL, Val);
      ProcessDrawEvents();      
#endif
      
    }
    
    ListDispose (hList); 
    SaveHashTable();    
    HashTableDispose (ghHash);  
  double t2 = Timer ();    

#if defined(WITH_PROGRESS) 
  SetCtrlAttribute(ghPanel, PANEL_PROGRESSBAR,ATTR_VISIBLE, 0);
#endif  

  sprintf(stmp, "\nA total of %d .c files was found.", gFound);
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp);    

  sprintf(stmp, "%d of them were NOT analyzed due to compilation errors.", gNotCompiled);
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp); 
  
  SetCtrlVal(ghPanel, PANEL_TIME, t2-t1);
}

// ----------------------------------------------------------------------------
/*
"\\dotnet\\GenericList\\genericlist.c",\                       
"\\dotnet\\Hashtable\\hashtable.c",\                             
"\\dotnet\\RegEx\\DotNetRegularExpressions.c",\                
"\\dotnet\\SimpleTaskMonitor\\SimpleTaskMonitor.c",\             
"\\dotnet\\Timer\\timer.c",\                                   
"\\dotnet\\WebService\\GetInfoByZip.c",\                          
*/
static void RemoveBlackListed(ListType List){
  // genericlist, DotNetRegularExpressions, timer 

  char *Blacklist[]={\
"\\activex\\ie\\iedemo.c",\                                    
"\\apps\\spectrum\\spectrum.c",\                                 
"\\dotnet\\FileSystemWatcher\\FileSystemWatcher.c",\             
"\\realtime\\FileIORT\\FileIORT.c",\                             
"\\realtime\\LoopTimingRT\\main_rt.c",\                          
"\\realtime\\NetworkVariableRT\\NetworkVariableRT.c",\           
"\\realtime\\RS-232RT\\usesDebugViaCom.c",\                      
"\\realtime\\SymmetricMultiprocessingRT\\main_rt.c",\            
"\\realtime\\ThreadPriorityRT\\main_rt.c",\                      
"\\realtime\\WatchdogRT\\main_rt.c",\                            
"\\userint\\custctrl\\legend\\cklegend.c",\  
"\\dotnet\\mscorlib2.c",\
"\\dotnet\\mscorlib.c",\
};

#if defined (_WIN64)
    int NbFiles = (int)ListNumItems (List);
#else
    int NbFiles = ListNumItems (List);
#endif
  
  for(int i=0; i!=NbFiles; ++i){
    char File2Process[MAX_PATHNAME_LEN];  
    ListGetItem (List, File2Process, i+1);
    for(int j=0; j!=sizeof(Blacklist)/sizeof(Blacklist[0]); ++j){
      if(*Blacklist[j] && strstr (File2Process, Blacklist[j])){ 
        ListRemoveItem (List, 0, i+1);
        // Since we deleted the element pointed to by i we must take it into account with --i 
        i--;
        // DONE : once found, we remove Blacklist[j] from the array Blacklist to speed up things
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


#if 0
// ----------------------------------------------------------------------------
// SDK FindFirstFile() calls allow recursive directory walking. This is not the case with CVI GetFirstFile() 
static void WalkDirectoryTree2(const char RootDir[]) {
  
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
        WalkDirectoryTree2(NewRootDir);
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
      CollectFunctionCalls(File2Process);     
      gFound++;  
    } while (FindNextFile(hFind, &ffd));
  }
  FindClose(hFind);
}
#endif

// ----------------------------------------------------------------------------
//static void CollectFunctionCalls(const char dir[], const char *filename){ 
static void CollectFunctionCalls(const char *File2Process){ 
  
  // TODO : Find a way to get a safe access to clang.exe. Find out the CVI directory
  const char Path2Clang[]    = "C:\\Program Files (x86)\\National Instruments\\CVI2013\\bin\\clang\\2.9\\clang.exe";    
  const char ClangOptions[]  = "-cc1 -S -o"; 
  char  TempFileName[L_tmpnam];      
  tmpnam(TempFileName);
  
  // see ..\Debug\CC_options.txt for example    
  const char CCOptions[]  = " \ 
  -triple i386-pc-win32 -fno-caret-diagnostics -fno-gnu-keywords -fms-extensions -mms-bitfields  -fmath-errno -mdisable-fp-elim -Wno-microsoft -Werror=gnu -Werror=pointer-arith                                    \        
  -Wpointer-sign -Wunreachable-code -Wparentheses -Wunused-variable -Wuninitialized -Wdiv-by-zero -Warray-bounds -Wnull-dereference -Wignored-qualifiers -Wreturn-type -Wpanel-handle                               \
  -Wtautological-compare -Wempty-body -Wmissing-braces -Woverlength-strings -Wcast-align -Wmissing-noreturn -Winvalid-noreturn -Wshadow -Wswitch -Wswitch-enum -Wunused-function                                    \
  -Wunused-label -Wunused-parameter -Wunused-value -Wused-but-marked-unused -Wconversion -Wsign-compare -Wchar-subscripts -Wdeclaration-after-statement -Wtypecheck-convert -Wfloat-equal                           \
  -Wvla -Wattributes -Wunknown-attributes -Wdeprecated-declarations -Wpacked -Wpadded -Wcomment -Wformat -Wimplicit-int -Wimplicit-function-declaration -Wbuiltin-implicit-decl                                     \
  -Wmissing-declarations -Wmissing-field-initializers -Wtrigraphs -Wmultichar -Wlong-long -Wunknown-pragmas -Wignored-pragmas                                                                                       \
  -Wpragma-reset -Wpragma-on-off-switch -Wpragma-unused -Wend-of-directive -Wpragma-not-supported -Wunexpected-token -Wfour-char-constants -Wdiscard-qual -Wsign-conversion -Wvariadic-macros                       \
  -Wmacro-name -Wmacro-poisoning -Wmacro-redefined -Wbuiltin-macro-redefined -Wbuiltin-macro-undefined -Wunused-macros -Wgnu-designator -Wnonnull -Wconstant-conversion -Wout-of-scope-declarations                 \
  -Wstatic-non-static -Wconstant-too-large -W#warnings -Wundef -Winclude-next -Wextra-tokens -Wline-number -Wweak -Wmember-redeclare -Wexpected-semi -Wtoo-many-args -Wliteral-range                                \
  -Wunknown-escape -Wshift -Wchar-init -Wbraces-init -Wincomplete-field -Wtentative -Wordered-comparison -Wpointer-compare -Wstack-memory -Wexcess-elements -Wmissing-terminating -Willegal-storage                 \
  -Wclang -Wbackslash -Wdecl-param -Wnegative-to-unsigned -Wextern -Wconstant-logical-operand -Winitializer-overrides -Wvariadic-call-conv -Wmain -Womp-nested-parallel -Womp-repeated-ordered                      \
  -Wpointer-int-conv -Wint-conversion -Wpointer-conv -fdiagnostics-show-option -cvi-debugging=extended -Werror=implicit-function-declaration -Wuninitialized-runtime                                                \
";
  
  // see ..\Debug\CallCC.bat for example
  // -D__LIBPROT__                               
  const char ClangDefine[] = "         \
-D_CVI_=1300                           \
-D_NI_i386_=1                          \ 
-D_NI_mswin_=1                         \ 
-D_NI_mswin32_=1                       \  
-D_CVI_DEBUG_=1                        \
-D_CVI_EXE_=1                          \
-D_LINK_CVIRTE_=1                      \
-D_CVI_FDS_=1                          \
-D_CVI_USE_FUNCS_FOR_VARS_=1           \  
-D__DEFALIGN=8                         \
-D_NI_VC_=220                          \
-D_WIN32=1                             \
-DWIN32=1                              \
-D__WIN32__=1                          \
-D_WINDOWS=1                           \
-D__NT__=1                             \
-D_M_IX86=400                          \
-D__FLAT__=1                           \
-D_PUSHPOP_SUPPORTED=1                 \
-D_CVI_C99_EXTENSIONS_=1               \
-D_CVI_PROFILE_LEVEL_=0                \
-D_TARGET_FILE_VERSION_=\"1.0.0.0\"    \
-D_TARGET_PRODUCT_VERSION_=\"1.0.0.0\" \
-D_OPENMP=200505                       \
-D_CVI_CONFIGURATION_NAME_=\"Debug\"   \
-D_CVI_RTE_SHARED_=1                   \
-DWIN32_LEAN_AND_MEAN                  \
";
  
  // TODO : Find a way to build the paths below using the CVI installation directory
  // see ..\Debug\CC_options.txt for example    
  const char ClangIncludes[] ="\  
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\include\"                                \
-I \"C:\\Program Files (x86)\\National Instruments\\Shared\\CVI\\include\"                            \
-I \"C:\\ProgramData\\National Instruments\\CVI2013\\include\"                                        \
-I \"C:\\ProgramData\\National Instruments\\CVI\\include\"                                            \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\include\\ansi\"                          \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\include\\clang\\2.9\"                    \
-I \"C:\\Program Files (x86)\\IVI Foundation\\IVI\\Include\"                                          \
-I \"C:\\Program Files (x86)\\IVI Foundation\\VISA\\winnt\\include\"                                  \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\sdk\\include\"                           \
-I \"c:\\Users\\Public\\Documents\\National Instruments\\CVI2013\\samples\\udp\"                      \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\activex\\word\"                \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\activex\\excel\"               \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\custctrl\"                     \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\custctrl\\custsupp\"           \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\cvirtsup\"                     \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\datasock\"                     \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\daqUtil\"                      \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\printing\"                     \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\toolbox\"                      \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\reportgen\"                    \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\localui\"                      \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\instr\"                                  \
-I \"C:\\Program Files (x86)\\National Instruments\\Shared\\CVI\\toolslib\\custctrl\"                 \
-I \"C:\\Program Files (x86)\\National Instruments\\Shared\\CVI\\toolslib\\custctrl\\custsupp\"       \
-I \"C:\\Program Files (x86)\\National Instruments\\Shared\\CVI\\instr\"                              \
-I \"C:\\ProgramData\\National Instruments\\CVI2013\\instr\"                                          \
-I \"c:\\Program Files (x86)\\National Instruments\\CVI2013\\bin\"                                    \
-I \"c:\\ProgramData\\National Instruments\\CVI2013\\bin\"                                            \
-I \"c:\\Program Files (x86)\\National Instruments\\Shared\\CVI\\Bin\"                                \
-I \"C:\\Users\\Public\\Documents\\National Instruments\\CVI2013\\samples\\dotnet\"                   \  
";
// 
  
  char stmp[1024];    
  static int index=0;
  index++;
  DebugPrintf("%03d - Processing : %s", index, File2Process);       
  sprintf(stmp, "%03d - Processing : %s", index, File2Process);  
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp);  
  ProcessDrawEvents();
  DebugPrintf("%03d Processing : %s", index, File2Process);   
  /**/
  char command[10*1024];  
  // see cmd /? - /C execute then ends - /K execute then stay active  
  sprintf(command, "cmd.exe /C  \"    \"%s\" %s \"%s\" %s %s  %s  \"%s\"  \"   ", Path2Clang, ClangOptions, TempFileName, CCOptions, ClangDefine, ClangIncludes, File2Process);
  int hExec;       
  // LE_SHOWNORMAL or LE_HIDE
  if(LaunchExecutableEx (command, LE_HIDE, &hExec)>=0) {
  /**/
  
  /*
  char command[10*1024];                                  
  // see cmd /? - /C execute then ends - /K execute then stay active
  // see compile.txt for errors and warnings
  sprintf(command, "cmd.exe /K  \"    \"%s\" %s \"%s\" %s %s  %s  \"%s\"  2>compile.txt\"   ", Path2Clang, ClangOptions, TempFileName, CCOptions, ClangDefine, ClangIncludes, File2Process);
  
  int hExec;       
  // LE_SHOWNORMAL or LE_HIDE
  if(LaunchExecutableEx (command, LE_SHOWNORMAL, &hExec)>=0) {
  */
  
    while(!ExecutableHasTerminated (hExec)) {
      ;
    }
    RetireExecutableHandle(hExec);
  }
  
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
    *TempFileName='\0';
  }else{
    sprintf(stmp, " *** Error while compiling : \n%s", File2Process);  
    InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp);    
    gNotCompiled++;
  }
}

// ----------------------------------------------------------------------------
static void SaveHashTable(void){
 
  char ProjectDirName[MAX_PATHNAME_LEN];
  GetProjectDir (ProjectDirName);

  char PathName[MAX_PATHNAME_LEN];
  const char OutputFileName[] = "CrossTable.txt";
  sprintf(PathName, "%s\\%s", ProjectDirName, OutputFileName);
  
  char PathName_HTML[MAX_PATHNAME_LEN];
  const char OutputFileName_HTML[] = "CrossTable.html";
  sprintf(PathName_HTML, "%s\\%s", ProjectDirName, OutputFileName_HTML);
  
  FILE *hFile      = fopen (PathName, "w");
  FILE *hFile_HTML = fopen (PathName_HTML, "w");
  fprintf(hFile_HTML, "%s", Header_HTML);    
  
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
  
  char stmp[1024];    
  sprintf(stmp, "\nGenerated report file : ./%s", OutputFileName);  
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp);
  sprintf(stmp, "This file is comma separated and can be easily imported in MS Excel");  
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp); 
  
  sprintf(stmp, "\nGenerated report file : ./%s", OutputFileName_HTML);  
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp);
  sprintf(stmp, "This file is HTML5 compliant and can be opened with any browser");  
  InsertTextBoxLine (ghPanel, PANEL_TEXTBOX, -1, stmp); 
}
