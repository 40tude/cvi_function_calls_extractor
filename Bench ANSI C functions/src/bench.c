#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif //_MSC_VER

#include <windows.h>

#ifdef _CVI_
#include <utility.h>    
#endif //_CVI_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "hr_time.h"

#ifdef _MSC_VER
#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _MSC_VER

// ----------------------------------------------------------------------------
typedef void (*ptrTest)(void);

// ----------------------------------------------------------------------------
void Bench_sprintf(void);
void Bench_strcpy(void);      
void Bench_rand(void);           
void Bench_sin(void);        
void Bench_malloc_free(void);       
void Bench_calloc_free(void);        
void Bench_memset(void);            
void Bench_strlen(void); 
void Bench_strcat(void); 
void Bench_strcmp(void);  
void Bench_fprintf1(void);
void Bench_fprintf2(void);     
void Bench_printf(void);              

// ----------------------------------------------------------------------------
  FILE *ghLog=NULL;

// ----------------------------------------------------------------------------
// argc, argv are no longer mandatory in C  
int main() {

//#define BENCH_SUITE

#if defined(BENCH_SUITE)
  ptrTest ArrayOfTest[]={   \
Bench_printf,               \
Bench_fprintf1,             \
Bench_fprintf2,             \
Bench_sprintf,              \
Bench_strcpy,               \
Bench_rand,                 \
Bench_sin,                  \
Bench_malloc_free,          \
Bench_calloc_free,          \
Bench_memset,               \
Bench_strlen,               \
Bench_strcat,               \
Bench_strcmp,               \
};
#else
  ptrTest ArrayOfTest[]={Bench_rand};
#endif // BENCH_SUITE
  
#ifdef _CVI_         
  SetBreakOnLibraryErrors (0); 
#endif
  ghLog = fopen("./log.txt", "w"); 
#ifdef _CVI_       
  SetBreakOnLibraryErrors (1);   
#endif
  
  if(ghLog==NULL){
    puts("Log file is use.");
    puts("Strike ENTER to exit");
    getchar();
    return;
  }
  
  
#if _CVI_PROFILE_LEVEL_ > 0
  CVIProfSetCurrentThreadProfiling(1);
#endif // _CVI_PROFILE_LEVEL_
  
  for(int i=0; i!=sizeof(ArrayOfTest)/sizeof(ptrTest); ++i) ArrayOfTest[i]();
  
#if _CVI_PROFILE_LEVEL_ > 0
  CVIProfSetCurrentThreadProfiling(0);
#endif // _CVI_PROFILE_LEVEL_
  
  fclose(ghLog);
  puts("Strike ENTER to exit");
  getchar();
  
#ifdef _MSC_VER
  _CrtDumpMemoryLeaks();
#endif // _MSC_VER

  // return is no longer mandatory in C
}



// ----------------------------------------------------------------------------
void Bench_printf(void){

  // ----------------------------------------------------------------------------
  // Bench prolog
  const int NB_ITERATIONS   = 4000;      
  int       fake_optimizer  = 0;
  StopWatch s;

  // ----------------------------------------------------------------------------
  // Bench
  StartTimer(&s);
    for (int i = 0; i != NB_ITERATIONS; ++i) {
      // Do your bench here. Make sure to use fake_optimizer
      fake_optimizer+=printf("%05d - dDummy Msg # %07d\n", i, fake_optimizer);     
    }
  StopTimer(&s);


  // ----------------------------------------------------------------------------
  // Bench epilog
  // Make sure the optimizer keep fake_optimizer variable "alive"    
  printf("In %s - fake_optimizer = %d\n", __FUNCTION__, fake_optimizer);           
  printf("%s took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", __FUNCTION__, GetElapsedTime(&s));  
}



// ----------------------------------------------------------------------------
void Bench_fprintf1(void){

  // ----------------------------------------------------------------------------
  // Bench prolog
  const int NB_ITERATIONS   = 4000;        
  int       fake_optimizer  = 0;
  StopWatch s;

  // ----------------------------------------------------------------------------
  // Bench
  StartTimer(&s);
    for (int i = 0; i != NB_ITERATIONS; ++i) {
      // Do your bench here. Make sure to use fake_optimizer
      fake_optimizer+=fprintf(stdout, "%05d - dDummy Msg # %07d\n", i, fake_optimizer);     
    }
  StopTimer(&s);


  // ----------------------------------------------------------------------------
  // Bench epilog
  // Make sure the optimizer keep fake_optimizer variable "alive"    
  printf("In %s - fake_optimizer = %d\n", __FUNCTION__, fake_optimizer);           
  printf("%s took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", __FUNCTION__, GetElapsedTime(&s));  
}





// ----------------------------------------------------------------------------
void Bench_fprintf2(void){

  // ----------------------------------------------------------------------------
  // Bench prolog
  const int NB_ITERATIONS  = 500000;  
  char TmpFile[MAX_PATH];     

#ifdef _CVI_
  tmpnam (TmpFile);
#else
  char TmpPath[MAX_PATH]; 
  GetTempPath(MAX_PATH, TmpPath); 
  GetTempFileName(TmpPath, "", 0, TmpFile);
#endif // _CVI_
  
  FILE *hFile = fopen(TmpFile, "w");

  int       fake_optimizer=0;
  StopWatch s;

  
  // ----------------------------------------------------------------------------
  // Bench
  StartTimer(&s);
    for (int i = 0; i != NB_ITERATIONS; ++i) {
      // Do your bench here. Make sure to use fake_optimizer
      fake_optimizer+= fprintf (hFile, "Dummy Msg # %d\n", fake_optimizer);      
    }
  StopTimer(&s);


  // ----------------------------------------------------------------------------
  // Bench epilog
  // Make sure the optimizer keep fake_optimizer variable "alive"    
  printf("In %s - fake_optimizer = %d\n", __FUNCTION__, fake_optimizer);           
  printf("%s took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", __FUNCTION__, GetElapsedTime(&s));  
  
  fclose(hFile);
  remove(TmpFile);
}


// ----------------------------------------------------------------------------
void Bench_strcmp(void){

  // ----------------------------------------------------------------------------
  // Bench prolog
  const int NB_STRINGS  = 100000;      
  char **Array          = (char**)(malloc(NB_STRINGS*sizeof(char*)));    

  // fill the strings pointed to by array elements
  for(int i=0; i!=NB_STRINGS; ++i){
    int end = 4096;
    Array[i] = (char*)malloc(end*sizeof(char));
    memset(Array[i], 'A', end*sizeof(char));
    Array[i][end-1] = '\0';
  }

  int       fake_optimizer=0;
  StopWatch s;


  // ----------------------------------------------------------------------------
  // Bench
  StartTimer(&s);
    for (int i = 0; i != NB_STRINGS; ++i) {
      // Do your bench here. Make sure to use fake_optimizer
      fake_optimizer += strcmp(Array[0], Array[i]);
    }
  StopTimer(&s);


  // ----------------------------------------------------------------------------
  // Bench epilog
  // Make sure the optimizer keep fake_optimizer variable "alive"    
  printf("In %s - fake_optimizer = %d\n", __FUNCTION__, fake_optimizer);           
  printf("%s took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", __FUNCTION__, GetElapsedTime(&s));  
  
  for(int i=0; i!=NB_STRINGS; ++i){
    free(Array[i]);
  }
  free(Array);
}


// ----------------------------------------------------------------------------
void Bench_strcat(void){
  
  // ----------------------------------------------------------------------------
  // Bench prolog
  const int NB_STRINGS  = 1000;  
  char **Array          = (char**)(malloc(NB_STRINGS*sizeof(char*)));    

  // fill the strings pointed to by array elements
  int TotalSize = 0;
  for(int i=0; i!=NB_STRINGS; ++i){
    int end = 4096;
    Array[i] = (char*)malloc(end*sizeof(char));
    memset(Array[i], 'A', end*sizeof(char));
    Array[i][end-1] = '\0';
    TotalSize +=end;
  }

  char *fake_optimizer = (char*)malloc(TotalSize*sizeof(char));
  fake_optimizer[0] = '\0';
  StopWatch s;
  
  
  // ----------------------------------------------------------------------------
  // Bench
  StartTimer(&s);
    // Do your bench here. Make sure to use fake_optimizer
    for(int i=0; i!=NB_STRINGS; ++i) {     
      strcat(fake_optimizer, Array[i]);
    }
  StopTimer(&s);

  // ----------------------------------------------------------------------------
  // Bench epilog
  // Make sure the optimizer keep fake_optimizer variable "alive"    
  printf("In %s - fake_optimizer[0] = %c\n", __FUNCTION__, fake_optimizer[0]);           
  printf("%s took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", __FUNCTION__, GetElapsedTime(&s));  
  
  for(int i=0; i!=NB_STRINGS; ++i){
    free(Array[i]);
  }
  free(Array);
  free(fake_optimizer);
}



// ----------------------------------------------------------------------------
void Bench_strlen(void){
  
  // ----------------------------------------------------------------------------
  // Bench prolog
  const int NB_ITERATIONS = 1000;    
  const int NB_STRINGS    = 1000;
  
  // array of ptr to char
  char **Array = (char**)(malloc(NB_STRINGS*sizeof(char*)));
  
  // fill the strings pointed to by array elements
  for(int i=0; i!=NB_STRINGS; ++i){
    int end = 4096;
    Array[i] = (char*)malloc(end*sizeof(char));
    memset(Array[i], 'A', end*sizeof(char));
    Array[i][end-1] = '\0';
  }

  int       fake_optimizer=0;     
  StopWatch s;
  
  // ----------------------------------------------------------------------------
  // Bench
  StartTimer(&s);
    for (int i = 0; i != NB_ITERATIONS; ++i) {
      for(int j=0; j!=NB_STRINGS; ++j) {
        // use fake_optimizer somehow. Does'nt matter if it overflows 
        fake_optimizer += strlen(Array[i]);
      }
    }
  StopTimer(&s);
  
  // ----------------------------------------------------------------------------
  // Bench epilog
  // Make sure the optimizer keep fake_optimizer variable "alive"    
  printf("In %s - fake_optimizer = %d\n", __FUNCTION__, fake_optimizer);
  printf("%s took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", __FUNCTION__, GetElapsedTime(&s));  
  
  for(int i=0; i!=NB_STRINGS; ++i){
    free(Array[i]);
  }
  free(Array);
}

// ----------------------------------------------------------------------------
void Bench_memset(void){
  
  // ----------------------------------------------------------------------------
  // Bench prolog
  const int     NB_ITERATIONS   = 200000;      
  unsigned char MyArray[100000];
  int           fake_optimizer  = 0;           
  StopWatch     s;
  
  // ----------------------------------------------------------------------------
  // Bench
  StartTimer(&s);
    for (int i = 0; i != NB_ITERATIONS; ++i) {
      // we are adding values of pointer on purpose
      fake_optimizer += (int)memset(MyArray, i%256, sizeof(MyArray));
    }
  StopTimer(&s);
  
  // ----------------------------------------------------------------------------
  // Bench epilog
  // Make sure the optimizer keep fake_optimizer variable "alive"    
  printf("In %s - fake_optimizer = %d\n", __FUNCTION__, fake_optimizer);           
  printf("%s took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", __FUNCTION__, GetElapsedTime(&s));  
}


// ----------------------------------------------------------------------------
void Bench_calloc_free(void){
  
  // ----------------------------------------------------------------------------
  // Bench prolog
  const int NB_STRINGS      = 20000;    
  char      **Array         = (char**)(malloc(NB_STRINGS*sizeof(char*)));
  int       fake_optimizer  = 0;
  StopWatch s;
  
  // ----------------------------------------------------------------------------
  // Bench
  StartTimer(&s);
    for(int i=0; i!=NB_STRINGS; ++i){
      Array[i] = (char*)calloc((1+(2<<14)), sizeof(char)); 
      fake_optimizer += Array[i][0];    
    }
  StopTimer(&s);
  // ----------------------------------------------------------------------------
  printf("In %s - fake_optimizer = %d\n", __FUNCTION__, fake_optimizer);       
  printf("calloc() section from %s() took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", "calloc", GetElapsedTime(&s));  
  
  
  StartTimer(&s);   
    for(int i=0; i!=NB_STRINGS; ++i){
      free(Array[i]);
    }
  StopTimer(&s);
  
  // ----------------------------------------------------------------------------       
  // Bench epilog              
  // Make sure the optimizer keep fake_optimizer variable "alive"    
  printf("free() section   from %s() took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", "free", GetElapsedTime(&s));  
  free(Array);
}


// ----------------------------------------------------------------------------
void Bench_malloc_free(void){
  
  // ----------------------------------------------------------------------------
  // Bench prolog
  const int NB_STRINGS      = 20000;    
  char      **Array         = (char**)(malloc(NB_STRINGS*sizeof(char*)));
  int       fake_optimizer  = 0;    
  StopWatch s;
  
  // ----------------------------------------------------------------------------
  // Bench
  StartTimer(&s);
    for(int i=0; i!=NB_STRINGS; ++i){
      Array[i] = (char*)malloc((1+(2<<14))*sizeof(char)); 
      fake_optimizer += Array[i][0]; 
    }
  StopTimer(&s);
  // ----------------------------------------------------------------------------
  printf("In %s - fake_optimizer = %d\n", __FUNCTION__, fake_optimizer);         
  printf("malloc() section from %s() took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", "malloc", GetElapsedTime(&s));  
  
  StartTimer(&s);   
    for(int i=0; i!=NB_STRINGS; ++i){
      free(Array[i]);
    }
  StopTimer(&s);

  // ----------------------------------------------------------------------------
  // Bench epilog
  printf("free() section   from %s() took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", "free", GetElapsedTime(&s));  
  free(Array);
}



// ----------------------------------------------------------------------------
void Bench_sprintf(void){
  
  // ----------------------------------------------------------------------------
  // Bench prolog
  const int NB_ITERATIONS   = 1000000;      
  int       fake_optimizer  = 0;                  
  StopWatch s;
  
  // ----------------------------------------------------------------------------
  // Bench
  StartTimer(&s);
    for (int i = 0; i != NB_ITERATIONS; ++i) {
      char tmp[44];     
      fake_optimizer += sprintf(tmp, "%d %d", i, i);
    }
  StopTimer(&s);
  
  // ----------------------------------------------------------------------------
  // Bench epilog
  printf("In %s - fake_optimizer = %d\n", __FUNCTION__, fake_optimizer);      
  printf("%s() took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", __FUNCTION__, GetElapsedTime(&s));  
}

// ----------------------------------------------------------------------------
void Bench_strcpy(void){

  // ----------------------------------------------------------------------------
  // Bench prolog
  const int NB_ITERATIONS = 1000000;      
  char source[100]        = "";
  
  for(int i=0; i!=100; ++i){
    source[i] ='A' + (char)i%26; 
  }
  source[99]='\0';

  int       fake_optimizer=0;
  StopWatch s;
  
  
  // ----------------------------------------------------------------------------
  // Bench
  StartTimer(&s);
    for (int i = 0; i != NB_ITERATIONS; ++i) {
      char target[100];
      // we add pointer values on purpose
      fake_optimizer += (int)strcpy(target, source);
    }
  StopTimer(&s);
  
  
  // ----------------------------------------------------------------------------
  // Bench epilog
  printf("In %s - fake_optimizer = %d\n", __FUNCTION__, fake_optimizer);         
  printf("%s() took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", __FUNCTION__, GetElapsedTime(&s));  
}

// ----------------------------------------------------------------------------
void Bench_rand(void){
  
  // ----------------------------------------------------------------------------
  // Bench prolog
  const int NB_ITERATIONS   = 10000000;            
  int       fake_optimizer  = 0;
  StopWatch s;
  
  srand((unsigned int)time(NULL));   
  
  
  // ----------------------------------------------------------------------------
  // Bench

  StartTimer(&s);
    for (int i = 0; i != NB_ITERATIONS; ++i) {
      // Be very carefull with rand. See http://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful 
      fake_optimizer += rand();
    }
  StopTimer(&s);
  
  
  // ----------------------------------------------------------------------------
  // Bench epilog
  // Make sure the optimizer keep fake_optimizer variable "alive"    
  printf("In %s - fake_optimizer = %d\n", __FUNCTION__, fake_optimizer);           
  printf("%s() took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", __FUNCTION__, GetElapsedTime(&s));  
}

// ----------------------------------------------------------------------------
void Bench_sin(void){
  
  // ----------------------------------------------------------------------------
  // Bench prolog
  const int NB_ITERATIONS   = 10000000;         
  double    fake_optimizer  = 0.0;
  StopWatch s;
  
  
  // ----------------------------------------------------------------------------
  // Bench
  StartTimer(&s);
    for (int i = 0; i != NB_ITERATIONS; ++i) {
      fake_optimizer += sin(i);
    }
  StopTimer(&s);
  
  
  // ----------------------------------------------------------------------------
  // Bench epilog
  // Make sure the optimizer keep fake_optimizer variable "alive"    
  printf("In %s - fake_optimizer = %f\n", __FUNCTION__, fake_optimizer);           
  printf("%s() took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", __FUNCTION__, GetElapsedTime(&s));  
}


/*
// ----------------------------------------------------------------------------
void Bench_template(void){

  // ----------------------------------------------------------------------------
  // Bench prolog
  const int NB_ITERATIONS   = 100;        
  int       fake_optimizer  = 0;
  StopWatch s;

  // ----------------------------------------------------------------------------
  // Bench

  StartTimer(&s);
    for (int i = 0; i != NB_ITERATIONS; ++i) {
      // Do your bench here. Make sure to use fake_optimizer
      fake_optimizer++;
    }
  StopTimer(&s);

  // ----------------------------------------------------------------------------
  // Bench epilog
  // Makes sure the optimizer keep fake_optimizer variable "alive"    
  printf("In %s - fake_optimizer = %d\n", __FUNCTION__, fake_optimizer);           
  printf("%s took : %.5f sec.\n", __FUNCTION__, GetElapsedTime(&s));
  fprintf(ghLog, "%s, %.5f\n", __FUNCTION__, GetElapsedTime(&s));            
}
*/
