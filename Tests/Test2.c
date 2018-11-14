//gcc -lkernel32 -I.. -L../Release -llibcryptohive Test2.c -o ../Release/Test2.exe
/* Test data:
0707b0cd9dd605d16dd9b0f5e59ce246ab39fd9413a889419b4a32385190c296e98f53f4a08347b50caf0448eba682b41bcdd20572d5a43a585ced0ad4dd1723d0da5a79af17aa8f949eae0f
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
Thread1: a206161b78840ab02479d7cedc4d10cbd8a9474a049417914c0a2de987365f00
Thread2: 1ddd6d4c6b9c41a89daa323b1e140e62d5ebf40a5962028cd1b4acd68deed830
*/
#include <windows.h>
#include <stdio.h>
#include <pthread.h>
#include <libcryptohive.h>

unsigned char input[2][blobLen] = {0};
unsigned char output[2][outputLen] = {0};

pthread_t T1, T2;
int F1 = 0, F2 = 0;

void *Thread1(void* param) {
  cryptohive_create();
  cryptohive_hash(input[0], output[0], blobLen);
  printf("Thread1: ");
  for(int i = 0; i < outputLen; i++)
    printf("%02hhx", output[0][i]);
  putchar('\n');
  cryptohive_destroy();
  F1 = 0;
  return NULL;
}

void *Thread2(void* param) {
  cryptohive_create();
  cryptohive_hash(input[1], output[1], blobLen);
  printf("Thread2: ");
  for(int i = 0; i < outputLen; i++)
    printf("%02hhx", output[1][i]);
  putchar('\n');
  cryptohive_destroy();
  F2 = 0;
  return NULL;
}

int main(void) {
  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < blobLen; j++) {
      scanf("%2x", &input[i][j]);
      printf("%02hhx", input[i][j]);
    }
    putchar('\n');
  }
  putchar('\n');
  F1 = !pthread_create(&T1, NULL, Thread1, NULL);
  F2 = !pthread_create(&T2, NULL, Thread2, NULL);
  while(F1 || F2) Sleep(100);
  return 0;
}
