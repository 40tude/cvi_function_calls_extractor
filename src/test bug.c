#include <stdlib.h>
#include <omp.h>

int main(){

  size_t NbFiles;
#pragma omp parallel for default(none) shared(NbFiles)
  for(int i=0; i<NbFiles; ++i){
  }
}


