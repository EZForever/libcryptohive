//$TCC -I./ -L./Release/ -llibcryptohive Test.c -o ./Release/Test.exe
/* Test data:
0707b0cd9dd605d16dd9b0f5e59ce246ab39fd9413a889419b4a32385190c296e98f53f4a08347b50caf0448eba682b41bcdd20572d5a43a585ced0ad4dd1723d0da5a79af17aa8f949eae0f
a206161b78840ab02479d7cedc4d10cbd8a9474a049417914c0a2de987365f00
*/
#include <stdio.h>
#include <libcryptohive.h>

int main(void) {
  unsigned char input[blobLen] = {0};
  unsigned char output[outputLen] = {0};

  for(int i = 0; i < blobLen; i++) {
    scanf("%2x", &input[i]);
    printf("%02hhx", input[i]);
  }
  putchar('\n');
  cryptohive_create();
  cryptohive_hash(input, output, blobLen);
  for(int i = 0; i < outputLen; i++)
    printf("%02hhx", output[i]);
  putchar('\n');
  cryptohive_destroy();
  return 0;
}
