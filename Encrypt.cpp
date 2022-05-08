#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

bool isCoprime(short a, short b){
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


short chooseCoprime(short p, short row){
    short a = (p/3 + row) % p;
    while(1){
        if(isCoprime(p,a)){
            return a;
        }
        a = (a+1) % p;
    }
    return a;
}

short **Transpose(short **A, int row, int col){
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

void shiftRow(short **A, int row, int col){

    for(short i = 0 ; i < row ; i++){
        short a = chooseCoprime(col, i);
        short b = i; 
        short *newRow = new short[col];
        for(int j = 0 ; j < col ; j++){
            short f = (a * j + b)%col; 
            newRow[f] = A[i][j];
        }
        delete A[i];
        A[i] = newRow;
    }
}

void convolution(int **A, int row, int col){
    /*TODO*/
}
void shuffle(int **A, int row, int col){
    /*TODO*/
}

int main()
{
    short row,col;
    //string filename("CowRol")
    ifstream f,fR,fG,fB;
    f.open("RowCol.txt");
    f >> row >> col;
    f.close();
    fR.open("R.txt");
    fG.open("G.txt");
    fB.open("B.txt");
    
    short **R = new short*[row];
    short **G = new short*[row];
    short **B = new short*[row];
    for (short i = 0; i < row; i++) {
        R[i] = new short[col]{0};
        G[i] = new short[col]{0};
        B[i] = new short[col]{0};
    }
    cout << row << col <<endl;
    for(short i = 0; i < row ; i++){
        for(short j = 0 ; j < col; j++){
            fR >> R[i][j];
            fG >> G[i][j];
            fB >> B[i][j];
        }
    }
    fR.close();
    fG.close();
    fB.close();
    shiftRow(R, row, col);
    B = Transpose(B, row, col);
    shiftRow(B, col, row);

    /*
    for(short i = 0; i < row ; i++){
        for(short j = 0 ; j < col; j++){
            cout << B[i][j] << " ";
        }
        cout << endl;
    }*/
}