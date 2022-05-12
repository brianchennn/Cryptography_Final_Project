#include <iostream>
#include <fstream>
#include <cstring>
#include <random>
#include <chrono>
#include <vector>
#include <cmath>
using namespace std;

bool isCoprime(short a, short b){ // a,b 是否互質
    while(b != 0){

        short tmp = a % b;
        a = b;
        b = tmp;
        if(b == 1){
            return true;
        } 
    }
    return false;
}


short chooseCoprime(short p, short row){ // 以row當seed 找出 <p 並與p互質的數
    short a = (p/3 + row) % p;
    while(1){
        if(isCoprime(p,a)){
            return a;
        }
        a = (a+1) % p;
    }
    return a;
}

short **Transpose(short **A, int row, int col){ // Blue 專用

    short **newp = new short*[col];
    for (short i = 0; i < col; i++) {
        newp[i] = new short[row]{0};
        for(short j = 0; j < row; j++){
            newp[i][j] = A[j][i];
        }
    }
    for(int i = 0 ; i < row ; i++){
        delete A[i];
    }
    return newp;
    

}

short **shiftRow(short **A, int row, int col){ // 仿造AES

    for(short i = 0 ; i < row ; i++){
        
        short a = chooseCoprime(col, i);
        short b = i; 
        short *newRow = new short[col];
        
        for(int j = 0 ; j < col ; j++){
            newRow[(a * j + b)%col] = A[i][j];
            
        }

        delete A[i];
        A[i] = newRow;
    }
    return A;
}

short **shuffleRow(short **A, int row, int col, int seed){

    short **output = new short*[row];
    vector<int> Coprimes;
    for(int i = 0 ; i < row ; i++){
        if(isCoprime(i,row)){
            Coprimes.push_back(i);
        }
    }
    int a,b;
    a = Coprimes[seed % Coprimes.size()];
    b = (seed + row/2)%row;
    for(int i = 0 ; i < row ; i++){
        output[(a*i+b)%row] = A[i];
    }
    for(int i = 0 ; i < row ; i++){
        delete A[i];
    }
    return output;

}

short **convolution(short **A, short *kernel_33, int row, int col){

    short **output = new short*[row];
    for (short i = 0; i < row; i++) {
        output[i] = new short[col]{0};
    }
    for(int i = 0 ; i < row ; i++){
        for(int j = 0 ; j < col ; j++){
            short sum = 0;
            for(int k = 0 ; k < 3 ; k++){
                for(int l = 0 ; l < 3 ; l++){
                    if(i+k-1 >= 0 && i+k-1 < row && j+l-1 >= 0 && j+l-1 < col){
                        sum += A[i+k-1][j+l-1] * kernel_33[3*k+l];
                    }
                }
            }

            output[i][j] = sum % 256;
            if(output[i][j] < 0) output[i][j]+=256;

        }
    }
    for(int i = 0 ; i < row ; i++){
        delete A[i];
    }
    return output;

}

void shuffle(short arr[], short n, unsigned seed){
    /*TODO*/
    short newarr[n];
    vector<int> Coprimes;
    for(int i = 0 ; i < n ; i++){
        if(isCoprime(i,n)){
            Coprimes.push_back(i);
        }
    }
    int a,b;
    a = Coprimes[seed % Coprimes.size()];
    b = (seed + n/2)%n;
    for(int i = 0 ; i < n ; i++){
        newarr[i] = arr[(a * i + b)%n];
    }
    for(int i = 0 ; i < n ; i++){
        arr[i] = newarr[i];
    }

}

short *flatten(short **A, int row, int col){
    short *output = new short[row*col];
    for(int i = 0 ; i < row ; i++){
        for(int j = 0 ; j < col ; j++){
            output[i*col+j] = A[i][j];
        }
    }
    return output;
}

double entropy(vector<int> v, int total){
    double entropy = 0.0f;
    for(int i = 0 ; i < v.size() ; i++){
        if(v[i] == 0)
            continue;
        entropy -= ((double)v[i]/(double)total) * log2((double)v[i]/(double)total);
    }
    return entropy;
}

int find_max_entropy_pos(short *A, int A_length, int plaintext_length){
    
    float max_entropy;
    int max_entropy_pos;
    vector<int> frequency(256,0);
    for(int i = 0 ; i < plaintext_length ; i++){
        frequency[A[i]] ++;
    }
    max_entropy = entropy(frequency, plaintext_length);
    max_entropy_pos = 0;
    for(int i = plaintext_length ; i < A_length ; i++){

        frequency[A[i-plaintext_length]]--;
        frequency[A[i]]++;
        double Entropy = entropy(frequency,plaintext_length);
        if(max_entropy < Entropy){
            max_entropy = Entropy;
            max_entropy_pos = i - plaintext_length + 1;
        }
    }
    cout << "Maximum entropy: "<<max_entropy<<endl;
    return max_entropy_pos;
}

std::string getOsName()
{
    #ifdef _WIN32
    return "Windows 32-bit";
    #elif _WIN64
    return "Windows 64-bit";
    #elif __APPLE__ || __MACH__
    return "Mac OSX";
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #elif __unix || __unix__
    return "Unix";
    #else
    return "Other";
    #endif
}           

int main(int argc, char *argv[])
{
    std::time_t t1 = std::time(nullptr);
    if(argc != 4){
        printf("Usage:\n  %s plaintext.txt cipher.txt picture.jpg\n", argv[0]);
        return 1;
    }
    string py_cmd;
    string OS = getOsName();

    
    if(OS.find("Win") != string::npos){
        py_cmd = "python jpg2array.py " + string(argv[3]);
        system(py_cmd.c_str());

    }else{
        py_cmd = "python3 jpg2array.py " + string(argv[3]);
        system(py_cmd.c_str());
    }
    
    
    short row,col;
    vector<uint8_t> plaintext;
    char c;
    ifstream f,fRGB,fpt;
    fpt.open(argv[1],ios::binary);
    int count = 0 ;
    while(fpt.get(c)){
        count ++;
        //cout << short(c) << endl;
        plaintext.push_back(int(c));
    }
    cout << "Plaintext Length: " << count << endl;
    /*for(int i = 0 ; i < plaintext.size() ;i ++){
        cout << plaintext[i] <<endl;
    }*/
    
    fRGB.open("RGB.txt");
    fRGB >> row >> col ;
    short **R = new short*[row];
    short **G = new short*[row];
    short **B = new short*[row];
    short **Rt = new short*[col];
    short **Gt = new short*[col];
    short **Bt = new short*[col];
    for (short i = 0; i < row; i++) {
        R[i] = new short[col]{0};
        G[i] = new short[col]{0};
        B[i] = new short[col]{0};
    }
    for (short i = 0; i < col; i++) {
        Rt[i] = new short[row]{0};
        Gt[i] = new short[row]{0};
        Bt[i] = new short[row]{0};
    }
    
    
    for(short i = 0; i < row ; i++){

        for(short j = 0 ; j < col; j++){
            fRGB >> R[i][j];
            fRGB >> G[i][j];
            fRGB >> B[i][j];
            //cout << B[i][j]<<endl;
        }
    }
    fRGB.close();
    
    short kernel1[9] = {2,3,5,7,11,13,17,19,23};
    unsigned int seed = 1;

    cout << "Processing Red" <<endl;
    shuffle(kernel1, 9, seed++);
    R = shiftRow(R, row, col);
    R = shuffleRow(R, row, col, seed);
    Rt = Transpose(R, row, col);
    Rt = shiftRow(Rt,col,row);
    R = Transpose(Rt, col, row);
    R = convolution(R, kernel1, row, col);
    
    cout << "Processing Green" <<endl;
    shuffle(kernel1, 9, seed++);
    G = shiftRow(G, row, col);
    Gt = Transpose(G, row, col);
    Gt = shiftRow(Gt,col,row);
    G = Transpose(Gt, col, row);
    G = convolution(G, kernel1, row, col);
    
    cout << "Processing Blue" <<endl;
    shuffle(kernel1, 9, seed++);
    B = shiftRow(B, row, col);
    Bt = Transpose(B, row, col);
    Bt = shiftRow(Bt,col,row);
    B = Transpose(Bt, col, row);
    B = convolution(B, kernel1, row, col);

    ofstream fout;
    fout.open("random_output.txt");
    short *_1DArray = new short[3*row*col];
    for(int i = 0; i < row ; i++){
 
        for(int j = 0 ; j < col; j++){
            fout << R[i][j] << " " << G[i][j] << " " << G[i][j]  << " ";
            _1DArray[3*(i*col+j)] = R[i][j];
            _1DArray[3*(i*col+j)+1] = G[i][j];
            _1DArray[3*(i*col+j)+2] = B[i][j];
        }

        fout << endl;
    }
    cout << "Calculating entropy " << endl;
    int max_entropy_pos = find_max_entropy_pos(_1DArray, 3*row*col, plaintext.size());
    cout << "Max Entropy Position: "<< max_entropy_pos <<endl;
    vector<int> cipher(plaintext.size(),0);
    ofstream fc;
    fc.open(argv[2],ios::binary);
    for(int i = 0 ; i < plaintext.size() ; i++){
        cipher[i] = plaintext[i] ^ _1DArray[max_entropy_pos + i];
        //cout << cipher[i] << endl;
        fc << char(cipher[i]);
    }
    fc.close();
    std::time_t t2 = std::time(nullptr);
    cout << "Time: "<< int(t2-t1) << endl;
    cout << "\n end\n"<<endl;
}