//gcc -I.. -L../Release -llibcryptohive Test.c -o ../Release/Test.exe
/* Test data:
0807a9b7d1d6053f1319632703fcfcd2cebc44d8ab44a6a03ae44d8f15af6217d1e09285e473f900000000fc09deabf58b6f1dcaa8baac74dd7419d5d610ec38cf50296a070b938f8fa81004
6902d6d809395477009c1aa24338de7e42502b5980264c2a0992a792b3543ac5

09099180b0df0553f6ddfa243e9bd917f088aefdc116380e9314df0a04acf8197ba92e33b17b2a931f0f4377b256cac19ec4f1e251d7ddcbdba8f85cce6a699107a8df054564b919bb398c0a
85016ef1d13631c4e1a8b86b1304ed7446dc94cb359f3d182aeea4689599e500
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
