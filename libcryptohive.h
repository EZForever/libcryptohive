#ifndef __LIBCRYPTOHIVE_H__
#define __LIBCRYPTOHIVE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//inputLen: Maximum length of input buffer.
#define inputLenMax 84

//blobLen: Current length of a Monero blob.
#define blobLen 76

//outputLen: Minimum length of output buffer.
#define outputLen 32

//cryptohive_create: Call before any _hash / _p* call.
void cryptohive_create(void);

//cryptohive_destroy: Call before thread ends.
void cryptohive_destroy(void);

//cryptohive_hash_v{012}: ...
void cryptohive_hash_v0(unsigned char input[], unsigned char output[], uint32_t inputLen);
void cryptohive_hash_v1(unsigned char input[], unsigned char output[], uint32_t inputLen);
void cryptohive_hash_v2(unsigned char input[], unsigned char output[], uint32_t inputLen);

//cryptohive_hash: Determine which hash function to call.
void cryptohive_hash(unsigned char input[], unsigned char output[], uint32_t inputLen);

//cryptohive_p{Input|Output}: return a pointer to internal buffer.
unsigned char *cryptohive_pInput(void);
unsigned char *cryptohive_pOutput(void);

//cryptohive_hash_v{012}_Q: Run hash function without any other operations.
void cryptohive_hash_v0_Q(void);
void cryptohive_hash_v1_Q(void);
void cryptohive_hash_v2_Q(void);

#ifdef __cplusplus
}
#endif

#endif //__LIBCRYPTOHIVE_H__