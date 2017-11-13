#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
//#include <string.h>
//#define BLOCK_SIZE 8

char* hash(FILE *f) {
  //It takes a FILE * which is an open file to read from.
  //Instead of using scanf you should read one byte at a time from the file
  //using fread.
  //The block size is predefined to be 8 bytes.
  char *hash_val;
  hash_val = malloc(sizeof(char) * BLOCK_SIZE);

  for(int i = 0; i<BLOCK_SIZE;i++){
    hash_val[i] = '\0';
  }
  char hs;
  int j =0;
  while(fread(&hs, sizeof(short), 1, f) != 0){
    if(j>= BLOCK_SIZE){
      j=0;
    }
    hash_val[j] = hash_val[j] ^ hs;
    //printf("%c\n", hash_val[j]);
    j++;
  }
  int e;
  e = fclose(f);
  if(e != 0){
      fprintf(stderr, "Error:fclose failed\n" );
  }
  return hash_val;

}

//test
void show_hash(char *hash_val, long block_size) {
    for(int i = 0; i < block_size; i++) {
        printf("%.2hhx ", hash_val[i]);
    }
    printf("\n");
}

/*
int main(int argc, char const *argv[]) {
  if(argc == 2){
    FILE *f;
    char * hash_val;
    f = fopen(argv[1],"rb");
    if(f == NULL){
      perror("fopen");
    }
    hash_val = hash(f);
    show_hash(hash_val, BLOCK_SIZE);
  }

  return 0;
}
*/
