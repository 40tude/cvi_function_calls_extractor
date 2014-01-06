#include <omp.h>
#include "toolbox.h"

// ----------------------------------------------------------------------------
static void GenerateWords(char *name);
static void InsertInTable(const char *name);
static void DisplayWords(void);

// ----------------------------------------------------------------------------
static const int kNbLettersMax  = 20;
static const int kNbWords       = 1024;  

static HashTableType  ghHash;   

// ----------------------------------------------------------------------------
int main(){
  
  HashTableCreate (10, C_STRING_KEY, 0, sizeof(int), &ghHash);      
  #pragma omp parallel for default(none) shared(ghHash)
  for(int i=0; i<kNbWords; ++i){
    char Name2Process[kNbLettersMax];  
    GenerateWords(Name2Process);
    
    
      InsertInTable(Name2Process); 
    
    /*
    // Does'nt work either
    int CurrVal;
    HashTableGetItem (ghHash, Name2Process, &CurrVal, sizeof(int));
    CurrVal++;
    HashTableInsertItem(ghHash, Name2Process, &CurrVal); 
    */
    
  }
  DisplayWords();
  HashTableDispose (ghHash); 
  getchar();
}

// ----------------------------------------------------------------------------
static void GenerateWords(char *name){
  
  char letter = (char) Random (0, 25);   
  int len = (int) Random (1, kNbLettersMax);   
  for(int i=0; i!=len; ++i){
    *name++ = 'A'+letter;
  }
  *name='\0';
}

// ----------------------------------------------------------------------------
static void InsertInTable(const char *name){
  
  int CurrVal;
  HashTableGetItem (ghHash, name, &CurrVal, sizeof(int));
  CurrVal++;
  #pragma omp critical 
  HashTableInsertItem(ghHash, name, &CurrVal); 
}

// ----------------------------------------------------------------------------
static void DisplayWords(void){
 
  HashTableIterator iter;   
  for (int status = HashTableIteratorCreate(ghHash, &iter); status >= 0 && status != HASH_TABLE_END; status = HashTableIteratorAdvance(ghHash, iter)) {
    int value;
    char key[256]; 
    status = HashTableIteratorGetItem(ghHash, iter, &key, sizeof(key)/sizeof(char), &value, sizeof(int));
    printf("%s, %d\n", key, value);
  }
  HashTableIteratorDispose(ghHash, iter);
}
