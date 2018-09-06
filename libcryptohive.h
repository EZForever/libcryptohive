#ifndef __LIBCRYPTOHIVE_H__
#define __LIBCRYPTOHIVE_H__

//inputLen£ºMaximum length of input buffer.
#define inputLenMax 84

//blobLen: Current length if a blob.
#define blobLen 76

//outputLen: Minimum ength of input buffer.
#define outputLen 32

//cryptohive_create: Call before any cryptohive_hash call.
void cryptohive_create(void);

//cryptohive_destroy: Call before program ends.
void cryptohive_destroy(void);

//cryptohive_hash_v0: ...
void cryptohive_hash_v0(unsigned char input[], unsigned char output[], uint32_t inputLen);

//cryptohive_hash_v1: ...
void cryptohive_hash_v1(unsigned char input[], unsigned char output[], uint32_t inputLen);

//cryptohive_hash: Determine which hash function to call.
void cryptohive_hash(unsigned char input[], unsigned char output[], uint32_t inputLen);

#endif //__LIBCRYPTOHIVE_H__