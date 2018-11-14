//gcc -O3 -I. -L./Release -llibcryptohive TestSpeed.c -o ./Release/TestSpeed.exe
/* Test data:
16 0707998e9edc055c022dd408ffe8e243fe9e6fb24e38a63c18873f492e34ffd2d0df6e8d8f569b0000000080a0d1d017dcc6117b6a9ba2f072074cb4e780e8f9fcc1f4b35f95febda8ae9801
a206161b78840ab02479d7cedc4d10cbd8a9474a049417914c0a2de987365f00
*/
#include <stdio.h>
#include <libcryptohive.h>

int main(void) {
  cryptohive_create();
  unsigned char *input = cryptohive_pInput();
  unsigned char *output = cryptohive_pOutput();
  uint32_t Loop = 1;

  scanf("%d", &Loop);
  for(int i = 0; i < blobLen; i++)
    scanf("%2x", &input[i]);

  void (*fHash)(void) = input[0] == 7 ? &cryptohive_hash_v1_Q : &cryptohive_hash_v0_Q;

  for(int i = 0; i < Loop; i++) fHash();

  for(int i = 0; i < outputLen; i++)
    printf("%02hhx", output[i]);
  putchar('\n');
  cryptohive_destroy();
  return 0;
}
