//gcc -O3 -I. -L./Release -llibcryptohive TestHash.c -o ./Release/TestHash.exe
/* Test data:
ffffff00:0707998e9edc055c022dd408ffe8e243fe9e6fb24e38a63c18873f492e34ffd2d0df6e8d8f569b0000000080a0d1d017dcc6117b6a9ba2f072074cb4e780e8f9fcc1f4b35f95febda8ae9801
442a496b:7252d3b278a93c80678ec93454dfa9e0a4f9329e74201a7bbf271c359d112f00
*/
#include <stdio.h>
#include <libcryptohive.h>

#define targetLen 8

//Translated from CoinHive's CryptonightWASMWrapper.prototype.meetsTarget
int MeetsTarget(unsigned char *output, unsigned char *target) {
  for(int i = 0; i < targetLen; i++) {
    int hi = outputLen - i - 1;
    int ti = targetLen - i - 1;
    if(output[hi] > target[ti]) return 0;
      else if(output[hi] < target[ti]) return 1;
  }
  return 0;
}

int main(void) {
  cryptohive_create();

  unsigned char target[targetLen] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  for(int i = targetLen - 4; i < targetLen; i++)
    scanf("%2x", &target[i]);

  unsigned char *input = cryptohive_pInput();
  unsigned char *output = cryptohive_pOutput();
  scanf("%*c"); //Skip ':'
  for(int i = 0; i < blobLen; i++)
    scanf("%2x", &input[i]);

  uint32_t *nonce = (uint32_t *)&input[39];
  //*nonce = (uint32_t)&nonce; //-Wpointer-to-int-cast on x64
  __asm__ __volatile__("rdtsc":"=a"(*nonce)); //Initial value; differs each thread

  void (*fHash)(void) = input[0] == 7 ? &cryptohive_hash_v1_Q : &cryptohive_hash_v0_Q;

  do {
    //XorShift32 from: https://en.wikipedia.org/wiki/Xorshift#Example_implementation
    *nonce ^= *nonce << 13; *nonce ^= *nonce >> 17; *nonce ^= *nonce << 5;
    //printf("%08x\n", *nonce);
    fHash();
  } while(!MeetsTarget(output, target));

  for(int i = 39; i < 39 + 4; i++)
    printf("%02hhx", input[i]);  //Big-endian
  putchar(':');
  for(int i = 0; i < outputLen; i++)
    printf("%02hhx", output[i]);
  putchar('\n');
  cryptohive_destroy();
  return 0;
}
