# libcryptohive #
兼容CoinHive的CryptoNight库  
**现在支持变种2**  
  
## 停止维护 ##
[CoinHive 关门大吉了。](https://jsecoin.com/coinhive-closing-down/)
  
## 蛤？ ##
自从~~四月七号~~门罗币硬分叉以来，CoinHive的脚本不再向非CoinHive矿池提交正确的hash。  
这导致原先的所有“CoinHive代理”软件全部停止工作（看[这里](https://github.com/cazala/coin-hive-stratum/issues/167)）。  
经过对代码的分析，我找到了hash错误的根源：CoinHive实现的CryptoNight库并不标准。  
例如，对于  
`blob = 0707998e9edc055c022dd408ffe8e243fe9e6fb24a63c18873f492e34ffd2d0df6e8f569b00080a0d1d017dcc6117b6a0720cb4e780fcc1f4b35f95febda8ae9801`  
`nonce = 0b100000`  
正确的结果应该为  
`result = 032fe2ad6afbadc7e1ef7be11da918f4702ca4491242836020d3d05cc8d71100`  
而CoinHive实现的结果为  
`result = 346ba98c32de42306f963abc8f1d896e762e6aa599fc3930fe82c8c3f4ac351a`  
但是，这一部分代码被编译为一个WebAssembly文件（~~[下载点这里](https://coinhive.com/lib/worker-v2.wasm)~~ 下载不可用），故而难以分析。  
所以我创建了这个repo，用来将这个WebAssembly文件转换为其他语言可用的库。  
  
## 如何使用 ##
*注意：我不保证能够正常编译、正常运行、正常计算，等等。
我只在自己的电脑上（Win10 x64，MinGW-w64，CoinHive 'worker-v8.wasm'）测试过。*  
把所有C文件编译到一起，生成 `.dll` / `.so` 文件。  
加上 `-O3` 开关可以得到大约7倍的性能提升。  
具体使用方式参见 `libcryptohive.h`和 `Tests/Test*.c`。  
  
## 开源许可 ##
`wasm-rt*` 修改自 [wasm2c](https://github.com/WebAssembly/wabt/tree/master/wasm2c)，遵守 [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)。  
其余文件： [GLWTPL](https://github.com/me-shaon/GLWTPL/blob/master/translations/LICENSE_zh-CN)  
