# libcryptohive #
A CoinHive-compatible CryptoNight hashing library
  
## What? ##
Since the April 7 Monero hard fork, CoinHive's script no longer submits the correct hash to a non-CoinHive pool.  
This caused all the "CoinHive proxy" software to stop working (see [here](https://github.com/cazala/coin-hive-stratum/issues/167)).  
After analyzing the code, I found the source of the hash error: the CryptoNight library implemented by CoinHive is not standard.  
For example, for  
`blob = 0707998e9edc055c022dd408ffe8e243fe9e6fb24a63c18873f492e34ffd2d0df6e8f569b00080a0d1d017dcc6117b6a0720cb4e780fcc1f4b35f95febda8ae9801`  
`nonce = 0b100000`  
The correct result should be  
`result = 032fe2ad6afbadc7e1ef7be11da918f4702ca4491242836020d3d05cc8d71100`  
The result of CoinHive implementation is  
`result = 346ba98c32de42306f963abc8f1d896e762e6aa599fc3930fe82c8c3f4ac351a`  
However, this part of the code is compiled into a WebAssembly file([Download](https://coinhive.com/lib/worker-v2.wasm)), which is difficult to analyze.  
So I created this repo to transform the WebAssembly file into a library available in other languages.  
  
## Usage ##
Important note: I do not guarantee that it can compile correctly, run correctly, calculate correctly, and so on.  
I only tested it on my own computer (Win10 x64, TCC 0.9.27 x86, CoinHive worker-v2.wasm).  
### 1. Download WebAssembly file ###
Download the file mentioned above and rename it to `coinhive.wasm` and put it in the repo directory.  
### 2. Download runtime files ###
Download all files with filenames beginning with `wasm-rt` from [here](https://github.com/WebAssembly/wabt/tree/master/wasm2c).
### 3. Generate the remaining files ###
`wasm2c coinhive.wasm -o coinhive.c`  
`wasm2c` can be downloaded from [here](https://github.com/WebAssembly/wabt/releases). 
### 4. Compile ###
Compile all C files (except `Test.c`) together to generate a DLL file.  
### 5. Reference in program ###
See `libcryptohive.h` and `Test.c`.

## License ##
[GLWTPL](https://github.com/me-shaon/GLWTPL/blob/master/LICENSE)  