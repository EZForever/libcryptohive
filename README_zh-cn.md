# libcryptohive #
����CoinHive��CryptoNight��
  
## �� ##
�Դ������ߺ����ޱ�Ӳ�ֲ�������CoinHive�Ľű��������CoinHive����ύ��ȷ��hash��  
�⵼��ԭ�ȵ����С�CoinHive�������ȫ��ֹͣ��������[����](https://github.com/cazala/coin-hive-stratum/issues/167)����  
�����Դ���ķ��������ҵ���hash����ĸ�Դ��CoinHiveʵ�ֵ�CryptoNight�Ⲣ����׼��  
���磬����  
`blob = 0707998e9edc055c022dd408ffe8e243fe9e6fb24a63c18873f492e34ffd2d0df6e8f569b00080a0d1d017dcc6117b6a0720cb4e780fcc1f4b35f95febda8ae9801`  
`nonce = 0b100000`  
��ȷ�Ľ��Ӧ��Ϊ  
`result = 032fe2ad6afbadc7e1ef7be11da918f4702ca4491242836020d3d05cc8d71100`  
��CoinHiveʵ�ֵĽ��Ϊ  
`result = 346ba98c32de42306f963abc8f1d896e762e6aa599fc3930fe82c8c3f4ac351a`  
���ǣ���һ���ִ��뱻����Ϊһ��WebAssembly�ļ���[���ص�����](https://coinhive.com/lib/worker-v2.wasm)�����ʶ����Է�����  
�����Ҵ��������repo�����������WebAssembly�ļ�ת��Ϊ�������Կ��õĿ⡣  
  
## ���ʹ�� ##
*ע�⣺�Ҳ���֤�ܹ��������롢�������С��������㣬�ȵȡ�
��ֻ���Լ��ĵ����ϣ�Win10 x64�� ~~TCC 0.9.27 x86~~ MinGW-w64��CoinHive worker-v2.wasm�����Թ���*  
### 1. ��������ʱ�ļ� ###
��[����](https://github.com/WebAssembly/wabt/tree/master/wasm2c)���������ļ����� `wasm-rt` ��ͷ���ļ���  
### 2. ���� ###
������C�ļ������� `Test.c`�����뵽һ������DLL�ļ���  
### 3. �ڳ��������� ###
�μ� `libcryptohive.h`�� `Test.c`��

## ��Դ��� ##
[��ף����ˡ��������֤](https://github.com/me-shaon/GLWTPL/blob/master/translations/LICENSE_zh-CN)  
