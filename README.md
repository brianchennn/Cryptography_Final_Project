# Cryptography_Final_Project

## Member
- 0716211 陳煜盛
- 0713211 洪嘉珊
- 0716225 洪瑋廷
- 0717030 紀品榕

## Code Execution (5/9 03:04更新)
python 版本為 3.10 太老可能 pillow 無法裝

- Windows:
先下載套件
```$ pip3 install pillow```
執行
```$ python jpg2array.py```
=> 產生 RGB.txt
=> 丟進 Encrypt.cpp
編譯
```$ g++ Encrypt.cpp```
```$ ./a.exe```

- Linux
```$ pip3 install pillow```
```$ python3 jpg2array.py```
```$ g++ Encrypt.cpp```
```$ ./a.out```
