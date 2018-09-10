//gcc -shared -static -O3 -fvisibility=hidden coinhive.c wasm-rt-impl.c libcryptohive.c -o ./Release/libcryptohive.dll -Wl,--out-implib,./Release/libcryptohive.a
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "wasm-rt-impl.h"
#include "coinhive.h"
#include "libcryptohive.h"

#define PAGE_NUM 0x100
#define PAGE_SIZE 0x10000

//Calcuated from JS code; see STACK.txt
#define ___tm_current 0x2c10
#define ___tm_timezone 0x2c40
#define DYNAMICTOP_PTR 0x2c50
#define STACKTOP 0x2c60
#define DYNAMIC_BASE 0x502c60

#define TRY(ret) { \
  wasm_rt_trap_t _TrapCode = wasm_rt_impl_try(); \
  if (_TrapCode != WASM_RT_TRAP_NONE) { \
    printf("[E][LIB] %s: TRAP %d\n", __func__, _TrapCode); \
    return ret; \
  } \
}

//--- Private definitions ---

THREAD struct {
  u32 ctx;
  wasm_rt_memory_t envMemory;
  wasm_rt_table_t envTable;
} cryptohive_ctx = {0};

u32 inputOffset = 0;
u32 outputOffset = 0;

//--- Import definitions ---

/* import: 'env' 'memory' */
THREAD wasm_rt_memory_t *Z_envZ_memory = NULL;

/* import: 'env' 'table' */
THREAD wasm_rt_table_t *Z_envZ_table = NULL;

/* import: 'env' 'tableBase' */
u32 envTableBase = 0;
u32 *Z_envZ_tableBaseZ_i = &envTableBase;

/* import: 'env' 'DYNAMICTOP_PTR' */
u32 envDYNAMICTOP_PTR = DYNAMICTOP_PTR;
u32 *Z_envZ_DYNAMICTOP_PTRZ_i = &envDYNAMICTOP_PTR;

/* import: 'env' 'STACKTOP' */
u32 envSTACKTOP = STACKTOP;
u32 *Z_envZ_STACKTOPZ_i = &envSTACKTOP;

/* import: 'env' 'abort' */
void envAbort(u32 what);
void (*Z_envZ_abortZ_vi)(u32) = &envAbort;

/* import: 'env' 'enlargeMemory' */
u32 envEnlargeMemory(void);
u32 (*Z_envZ_enlargeMemoryZ_iv)(void) = &envEnlargeMemory;

/* import: 'env' 'getTotalMemory' */
u32 envGetTotalMemory(void);
u32 (*Z_envZ_getTotalMemoryZ_iv)(void) = &envGetTotalMemory;

/* import: 'env' 'abortOnCannotGrowMemory' */
u32 envAbortOnCannotGrowMemory(void);
u32 (*Z_envZ_abortOnCannotGrowMemoryZ_iv)(void) = &envAbortOnCannotGrowMemory;

/* import: 'env' '___setErrNo' */
void envSetErrNo(u32);
void (*Z_envZ____setErrNoZ_vi)(u32) = &envSetErrNo;

/* import: 'env' '___syscall20' */
u32 envSyscall20(u32, u32);
u32 (*Z_envZ____syscall20Z_iii)(u32, u32) = &envSyscall20;

/* import: 'env' '_emscripten_memcpy_big' */
u32 envEmscripten_memcpy_big(u32, u32, u32);
u32 (*Z_envZ__emscripten_memcpy_bigZ_iiii)(u32, u32, u32) = &envEmscripten_memcpy_big;

/* import: 'env' '_ftime' */
u32 envFTime(u32);
u32 (*Z_envZ__ftimeZ_ii)(u32) = &envFTime;

/* import: 'env' '_gmtime' */
u32 envGMTime(u32);
u32 (*Z_envZ__gmtimeZ_ii)(u32) = envGMTime;

//--- Import fixup functions ---

__attribute__((noreturn)) void envAbort(u32 what) {
  //TODO: Process JSON data in 'what'.
  //printf("abort(%s). Build with -s ASSERTIONS=1 for more info.\n", (char*)what);
  puts("ABORTED");
  exit(1);
}

__attribute__((noreturn)) u32 envEnlargeMemory(void) {
  envAbortOnCannotGrowMemory();
}

u32 envGetTotalMemory(void) {
  return PAGE_NUM * PAGE_SIZE;
}

__attribute__((noreturn)) u32 envAbortOnCannotGrowMemory(void) {
  //envAbort("Cannot enlarge memory arrays. Either (1) compile with -s TOTAL_MEMORY=X with X higher than the current value " + TOTAL_MEMORY + ", (2) compile with -s ALLOW_MEMORY_GROWTH=1 which allows increasing the size at runtime, or (3) if you want malloc to return NULL (0) instead of this abort, compile with -s ABORTING_MALLOC=0 ");
  envAbort(0);
}

void envSetErrNo(u32 value) {
  //TODO: Translate this
  //if (Module["___errno_location"]) HEAP32[Module["___errno_location"]() >> 2] = value;
  //return value; //???
}

u32 envSyscall20(u32 which, u32 varargs) {
/* TODO: Translate this
    SYSCALLS.varargs = varargs;
    try {
        return PROCINFO.pid //42
    } catch(e) {
        if (typeof FS === "undefined" || !(e instanceof FS.ErrnoError)) abort(e);
        return - e.errno
    }
}
*/
  return 42;
}

u32 envEmscripten_memcpy_big(u32 dest, u32 src, u32 num) {
  memcpy(&cryptohive_ctx.envMemory.data[dest], &cryptohive_ctx.envMemory.data[src], num);
  return dest;
}

u32 envFTime(u32 p) {
  ftime((struct timeb *)&cryptohive_ctx.envMemory.data[p]);
  return 0;
}

u32 envGMTimeR(u32 _time, u32 tmPtr) {
  time_t timestamp = *(time_t *)&cryptohive_ctx.envMemory.data[_time] * 1000;
  //I can't get this to work
  //gmtime_r(&timestamp, (struct tm *)&cryptohive_ctx.envMemory.data[tmPtr]);
  gmtime_s((struct tm *)&cryptohive_ctx.envMemory.data[tmPtr], &timestamp);
  *(u32*)(&cryptohive_ctx.envMemory.data[tmPtr + 36]) = 0;
  *(u32*)(&cryptohive_ctx.envMemory.data[tmPtr + 40]) = ___tm_timezone;
  return tmPtr;
}

u32 envGMTime(u32 _time) {
  return envGMTimeR(_time, ___tm_current);
}

//--- Private functions ---

void _cryptohive_Input(unsigned char input[], uint32_t inputLen) {
  memcpy(&cryptohive_ctx.envMemory.data[inputOffset], input, inputLen);
}

void _cryptohive_Output(unsigned char output[]) {
  memcpy(output, &cryptohive_ctx.envMemory.data[outputOffset], outputLen);
}

//--- Public functions ---

#define EXPORT __attribute__((visibility("default")))

EXPORT void cryptohive_create(void) {
  Z_envZ_memory = &cryptohive_ctx.envMemory;
  Z_envZ_table = &cryptohive_ctx.envTable;

  wasm_rt_allocate_memory(Z_envZ_memory, PAGE_NUM, UINT32_MAX);
  wasm_rt_allocate_table(Z_envZ_table, 8u, 8u);
  init();
  strcpy((char*)&cryptohive_ctx.envMemory.data[___tm_timezone], "GMT");
  *(u32*)(&cryptohive_ctx.envMemory.data[DYNAMICTOP_PTR]) = DYNAMIC_BASE;

  TRY()
  inputOffset = (*WASM_RT_ADD_PREFIX(Z__mallocZ_ii))(inputLenMax);
  outputOffset = (*WASM_RT_ADD_PREFIX(Z__mallocZ_ii))(outputLen);
  cryptohive_ctx.ctx = (*WASM_RT_ADD_PREFIX(Z__cryptonight_createZ_iv))();
}

EXPORT void cryptohive_destroy(void) {
  (*WASM_RT_ADD_PREFIX(Z__cryptonight_destroyZ_vi))(cryptohive_ctx.ctx);
  free(cryptohive_ctx.envMemory.data);
  free(cryptohive_ctx.envTable.data);
}

EXPORT void cryptohive_hash_v0(unsigned char input[], unsigned char output[], uint32_t inputLen) {
  _cryptohive_Input(input, inputLen);
  TRY()
  (*WASM_RT_ADD_PREFIX(Z__cryptonight_hash_variant_0Z_viiii))(cryptohive_ctx.ctx, inputOffset, outputOffset, inputLen);
  _cryptohive_Output(output);
}

EXPORT void cryptohive_hash_v1(unsigned char input[], unsigned char output[], uint32_t inputLen) {
  _cryptohive_Input(input, inputLen);
  TRY()
  (*WASM_RT_ADD_PREFIX(Z__cryptonight_hash_variant_1Z_viiii))(cryptohive_ctx.ctx, inputOffset, outputOffset, inputLen);
  _cryptohive_Output(output);
}

EXPORT void cryptohive_hash(unsigned char input[], unsigned char output[], uint32_t inputLen) {
  (*(input[0] == 7 ? &cryptohive_hash_v1 : &cryptohive_hash_v0))(input, output, inputLen);
}

EXPORT unsigned char *cryptohive_pInput(void) {
  return (unsigned char *)&cryptohive_ctx.envMemory.data[inputOffset];
}

EXPORT unsigned char *cryptohive_pOutput(void) {
  return (unsigned char *)&cryptohive_ctx.envMemory.data[outputOffset];
}

EXPORT void cryptohive_hash_v0_Q(void) {
  (*WASM_RT_ADD_PREFIX(Z__cryptonight_hash_variant_0Z_viiii))(cryptohive_ctx.ctx, inputOffset, outputOffset, blobLen);
}

EXPORT void cryptohive_hash_v1_Q(void) {
  (*WASM_RT_ADD_PREFIX(Z__cryptonight_hash_variant_1Z_viiii))(cryptohive_ctx.ctx, inputOffset, outputOffset, blobLen);
}
