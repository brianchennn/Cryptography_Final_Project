# Cryptography_Final_Project

## Member
- 0716211 陳煜盛
- 0713211 洪嘉珊
- 0716225 洪瑋廷
- 0717030 紀品榕

## Code Execution (5/10 01:22更新)
python 版本為 3.10 太老可能 pillow 無法裝

- Windows:
```$ pip3 install pillow```
```$ python jpg2array.py```
```$ g++ Encrypt.cpp```
```$ ./a.exe plaintext.txt```

- Linux
```$ pip3 install pillow```
```$ python3 jpg2array.py```
```$ g++ Encrypt.cpp```
```$ ./a.out plaintext.txt```

- ```random_output.txt``` 
  - 這個檔案就是經過shiftRow,shiftColumn,convolution 所形成的 RGB 2D array
  - 格式為 R,G,B,R,G,B....
