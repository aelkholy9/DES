#include <iostream>
#include <vector>
#include <math.h>
#include <bitset>
#include <algorithm>
using namespace std;

string originalKey = "0001001100110100010101110111100110011011101111001101111111110001";
vector<string> KeyList_56;
string c,d;
vector<string> KeyList_48;
string M = "0000000100100011010001010110011110001001101010111100110111101111";
string ip_data;
string L,R;
vector<string> KeyList_LR;
// 1  >>>>> convert original 64bits key to 56bits and store it in the same original string
string PC1() {
int pc1[8][7] = {{57, 49, 41, 33, 25, 17, 9}, {1, 58, 50, 42, 34, 26, 18}, {10, 2, 59, 51, 43, 35, 27},
                {19, 11, 3, 60, 52, 44, 36}, {63, 55, 47, 39, 31, 23, 15}, {7, 62, 54, 46, 38, 30, 22}
                , {14, 6, 61, 53, 45, 37, 29}, {21, 13, 5, 28, 20, 12, 4}};
    string res = "";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 7; j++) {
            res += originalKey[pc1[i][j]-1];
        }
    }
    originalKey = res;
    return res;
}

// 2 >>>> split 56bits key to c and d
void split() {
    for (int i = 0; i < (int)originalKey.size() / 2; i++) {
        c += originalKey[i];
    }

    for (int i = (int)originalKey.size() / 2; i < (int)originalKey.size(); i++) {
        d += originalKey[i];
    }

}


// 3 >> get 16 keys from c,d and store them in KeyList_56 container
void shifts() {
    for (int i = 1; i <= 16; i++) {
        string tmpc = "", tmpd = "";
        if (i == 1 || i == 2 || i == 9 || i == 16) {
            tmpc += c[0];
            c = c.substr(1) + tmpc;
            tmpd += d[0];
            d = d.substr(1) + tmpd;
        } else {
            tmpc += c[0];
            tmpc += c[1];
            c = c.substr(2) + tmpc;
            tmpd += d[0];
            tmpd += d[1];
            d = d.substr(2) + tmpd;
        }
        KeyList_56.push_back(c+d);
        //cout << c << ' ' << d << endl;
    }
}


// 4 >> convert the 16 keys from 56bits to 48 bits and store them in KeyList_48
void PC2() {
    int pc2[8][6] = {{14, 17, 11, 24, 1, 5}, {3, 28, 15, 6, 21, 10}, {23, 19, 12, 4, 26, 8},
    {16, 7, 27, 20, 13, 2}, {41, 52, 31, 37, 47, 55}, {30, 40, 51, 45, 33, 48}
    , {44, 49, 39, 56, 34, 53}, {46, 42, 50, 36, 29, 32}};

    for(int key=0;key<KeyList_56.size();key++)
    {    string res = "";
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 6; j++) {
                res += KeyList_56[key][pc2[i][j]-1];
            }
        }
        KeyList_48.push_back(res);
    }
}


// 5 >> encode each 64-bit block of data
void IP(){
    int ip[8][8] = {{58, 50, 42, 34, 26, 18, 10, 2}, {60, 52, 44, 36, 28, 20, 12, 4}, {62, 54, 46, 38, 30, 22, 14, 6},
    {64, 56, 48, 40, 32, 24, 16, 8}, {57, 49, 41, 33, 25, 17, 9, 1}, {59, 51, 43, 35, 27, 19, 11, 3}
    , {61, 53, 45, 37, 29, 21, 13, 5}, {63, 55, 47, 39, 31, 23, 15, 7}};

    string res="";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            res += M[ip[i][j]-1];
        }
    }
    ip_data = res;
}


// 6 >> split IP_data and store the result in strings L and R ..
void split_ip()
{
    L="";R="";
    for (int i = 0; i < (int)ip_data.size() / 2; i++) {
        L += ip_data[i];
    }

    for (int i = (int)ip_data.size() / 2; i < (int)ip_data.size(); i++) {
        R += ip_data[i];
    }
}




// 7 >> proceed through 16 iterations to apply Ln = Rn-1 , Rn = Ln-1 + f(Rn-1,Kn) *** this part is composed of 4 functions


// ************** function that expands the 32bits R to 48bits R using E bit selection table
string eBitSelection(string R)
{
    int ebit[8][6] = {{32, 1, 2, 3, 4, 5}, {4, 5, 6, 7, 8, 9}, {8, 9, 10, 11, 12, 13},
    {12, 13, 14, 15, 16, 17}, {16, 17, 18, 19, 20, 21}, {20, 21, 22, 23, 24, 25}
    , {24, 25, 26, 27, 28, 29}, {28, 29, 30, 31, 32, 1}};

    string res="";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            res += R[ebit[i][j]-1];
        }
    }
    //R = res;
    return res;

}

// *********** xorr function
string xorr(string y,string z)
{
    string x="";
    for (int i=0;i<y.size();i++)
        x+= y[i]==z[i]? '0':'1';
    return x;
}


// ******** S_functions
string sfunction(string str)
{
    vector<string>blocks_8bit;
    // splitting the 48bit string to 6 blocks of 8 bits
    int y=0;
    for(int x=0;x<8;x++)
    {
        blocks_8bit.push_back(str.substr(y,6));
      // cout << blocks_8bit[blocks_8bit.size()-1] << ' ';
        y+=6;
    }

    // using s tables :---
    int s1[8][4][16] = {

        {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7}, {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},

        {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10}, {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},

        {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8}, {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},

        {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15}, {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},

        {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9}, {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},

        {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11}, {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},

        {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1}, {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},

        {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7}, {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}

    };



        string result="";
        for (int x=0;x<blocks_8bit.size();x++)
        {


            string firstLast_bits="";
            firstLast_bits += blocks_8bit[x][0];
            firstLast_bits += blocks_8bit[x][5];
            string middle = "";
            middle += blocks_8bit[x][1] ;
            middle += blocks_8bit[x][2] ;
            middle += blocks_8bit[x][3] ;
            middle += blocks_8bit[x][4];

            bitset<2>firstLast(firstLast_bits);
            bitset<4>mid(middle);

           int tmp = s1[x][firstLast.to_ulong()][mid.to_ulong()];
           bitset<4>tmpSet(tmp);
           result += tmpSet.to_string();

        }


    return result;

}

string applyPermutation(string target){
    int p[8][4] = {{16, 7, 20, 21}, {29, 12, 28, 17}, {1, 15, 23, 26},
                {5, 18, 31, 10}, {2, 8, 24, 14}, {32, 27, 3, 9}
                , {19, 13, 30, 6}, {22, 11, 4, 25}};
    string res = "";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            res += target[p[i][j]-1];
        }
    }

    return res;

}

// ***************** main function

string applyFormula()
{
    for (int i=0;i<KeyList_48.size();i++)
    {


        string tmpL = L, tmpR = R;
        L = R;
        string right_hand_side = sfunction(xorr(KeyList_48[i],eBitSelection(R)));
        string right_hand_side_permutation = applyPermutation(right_hand_side);
        R = xorr(right_hand_side_permutation,tmpL);
    }

    string finall = R + L ;
    return finall;
}


// 8 >> apply final permutation
string finalPermutation(string target)
{
    int IP1[8][8] = {{40, 8, 48, 16, 56, 24, 64, 32},{39, 7, 47, 15, 55, 23, 63, 31},{38, 6, 46, 14, 54, 22, 62, 30},{37, 5, 45, 13, 53, 21, 61, 29},
                    {36, 4, 44, 12, 52, 20, 60, 28},{35, 3, 43, 11, 51, 19, 59, 27},{34, 2, 42, 10, 50, 18, 58, 26},{33, 1, 41, 9, 49, 17, 57, 25}};

    string res = "";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            res += target[IP1[i][j]-1];
        }
    }

    return res;


}

int main()
{
    cout << "Original Text: " << M << endl;
    PC1();
    split();
    shifts();
    PC2();
    IP();
    split_ip();
    string finalString = applyFormula();
    //apply final permutation
    finalString = finalPermutation(finalString);
    cout << "Encrypted Text: " << finalString << endl;

    // Decrypt the encrypted text stored in finalString
    // changed the message M to the encrypted text and reversed the order of the keys
    M = finalString;
    reverse(KeyList_48.begin(),KeyList_48.end());
    IP();
    split_ip();
    finalString = applyFormula();
    finalString = finalPermutation(finalString);
    cout << "Decrypted Text: " << finalString << endl;
    return 0;
}
