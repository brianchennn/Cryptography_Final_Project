#include <iostream>
#include <fstream>
#include <cstring>
fewfe
#includ
    while(1){
        if(isCoprime(p,a)){
            return a;
        }
        a = (a+1) % p;
    }
    return a;
}

_8b **Transpose(_8b **A, int row, int col){ // Blue 專用

    _8b **output = new _8b*[col];
    for (int i = 0; i < col; i++) {
        output[i] = new _8b[row]{0};
        for(int j = 0; j < row; j++){
            output[i][j] = A[j][i];
        }
    }
    return output;
    

}

_8b **shiftRow(_8b **A, int row, int col){ // 仿造AES

    for(int i = 0 ; i < row ; i++){
        
        int a = chooseCoprime(col, i);
        int b = i; 
        _8b *newRow = new _8b[col];
        
        for(int j = 0 ; j < col ; j++){
            newRow[(a * j + b)%col] = A[i][j];
            
        }

        delete A[i];
        A[i] = newRow;
    }
    return A;
}

_8b **shuffleRow(_8b **A, int row, int col, int seed){

    _8b **output = new _8b*[row];
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

_8b **convolution(_8b **A, _8b *kernel_33, int row, int col){

    _8b **output = new _8b*[row];
    for (int i = 0; i < row; i++) {
        output[i] = new _8b[col]{0};
    }
    for(int i = 0 ; i < row ; i++){
        for(int j = 0 ; j < col ; j++){
            int sum = 0;
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
    /*for(int i = 0 ; i < row ; i++){
        A[i] = output[i];
        delete output[i];
    }*/
}

void shuffle(_8b arr[], _8b n, unsigned seed){
    /*TODO*/
    _8b newarr[n];
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


float entropy(vector<int> v, int total){
    float entropy = 0.0;
    for(int i = 0 ; i < v.size() ; i++){
        if(v[i] == 0)
            continue;
        entropy -= ((float)v[i]/(float)total) * log2((float)v[i]/(float)total);
    }

    return entropy;
}

int find_max_entropy_pos(_8b *A, int A_length, int plaintext_length){
    
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
        float Entropy = entropy(frequency,plaintext_length);
        if(max_entropy < Entropy){
            max_entropy = Entropy;
            max_entropy_pos = i - plaintext_length + 1;
        }
        //cout << max_entropy <<endl;
        if(max_entropy > 5.0f){
            break;
        }
    }
    cout << "Max entropy: "<<max_entropy<<endl;
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
        printf("Usage:\n  $ %s plaintext.txt cipher.txt image.jpg\nQuit\n", argv[0]);
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

    int row,col;
    vector<_8b> plaintext;
    char c;
    ifstream f,fRGB,fpt;
    fpt.open(argv[1],ios::binary);
    int count = 0;
    while(fpt.get(c)){
        count++;
        cout << short(c) << endl;
        plaintext.push_back(_8b(c));
    }
    cout << "count " << count << endl;
    /*for(int i = 0 ; i < plaintext.size() ;i ++){
        cout << plaintext[i] <<endl;
    }*/
    
    fRGB.open("RGB.txt");
    fRGB >> row >> col ;
    _8b **R = new _8b*[row];
    _8b **G = new _8b*[row];
    _8b **B = new _8b*[row];
    _8b **Rt = new _8b*[col];
    _8b **Gt = new _8b*[col];
    _8b **Bt = new _8b*[col];
    for (int i = 0; i < row; i++) {
        R[i] = new _8b[col]{0};
        G[i] = new _8b[col]{0};
        B[i] = new _8b[col]{0};
    }
    for (int i = 0; i < col; i++) {
        Rt[i] = new _8b[row]{0};
        Gt[i] = new _8b[row]{0};
        Bt[i] = new _8b[row]{0};
    }
    
    
    for(int i = 0; i < row ; i++){
        for(int j = 0 ; j < col; j++){
            fRGB >> R[i][j];
            fRGB >> G[i][j];
            fRGB >> B[i][j];
            //cout << B[i][j]<<endl;
        }
    }
    fRGB.close();
    
    _8b kernel1[9] = {2,3,5,7,11,13,17,19,23};
    unsigned int seed = 1;

    cout << "Processing Red\n";
    shuffle(kernel1, 9, seed++);
    /*R = shiftRow(R, row, col);
    R = shuffleRow(R, row, col, seed);
    Rt = Transpose(R, row, col);
    Rt = shiftRow(Rt,col,row);
    R = Transpose(Rt, col, row);
    R = convolution(R, kernel1, row, col);
    cout << "Processing Green\n";
    shuffle(kernel1, 9, seed++);
    G = shiftRow(G, row, col);
    Gt = Transpose(G, row, col);
    Gt = shiftRow(Gt,col,row);
    G = Transpose(Gt, col, row);
    G = convolution(G, kernel1, row, col);
    cout << "Processing Blue\n";
    shuffle(kernel1, 9, seed++);
    B = shiftRow(B, row, col);
    Bt = Transpose(B, row, col);
    Bt = shiftRow(Bt,col,row);
    B = Transpose(Bt, col, row);
    B = convolution(B, kernel1, row, col);*/

    ofstream fout;
    fout.open("random_output.txt");
    _8b *_1DArray = new _8b[3*row*col];
    for(int i = 0; i < row ; i++){
        //cout << i <<endl;
        for(int j = 0 ; j < col; j++){
            //cout << col * i + j << " ";
            fout << R[i][j] << " " << G[i][j] << " " << G[i][j]  << " ";
            _1DArray[3*(i*col+j)] = R[i][j];
            _1DArray[3*(i*col+j)+1] = G[i][j];
            _1DArray[3*(i*col+j)+2] = B[i][j];
        }
        //cout << 1 <<endl;
        fout << endl;
    }
    int max_entropy_pos = find_max_entropy_pos(_1DArray, 3*row*col, plaintext.size());
    cout << "Position: " << max_entropy_pos << endl;
    vector<_8b> cipher(plaintext.size(),0);
    ofstream fc;
    fc.open(argv[2], ios::binary);
    for(int i = 0 ; i < plaintext.size() ; i++){
        cipher[i] = plaintext[i] ^ _1DArray[max_entropy_pos + i];
        //cout << short(plaintext[i]) << " " << _1DArray[max_entropy_pos + i] << endl;
        //cout << short(cipher[i]) ;
        fc << _8b(cipher[i]);
    }
    fc.close();
    std::time_t t2 = std::time(nullptr);
    cout << "Time: "<< t2-t1 <<" sec "<<endl;
    cout << "\n end\n"<<endl;
}