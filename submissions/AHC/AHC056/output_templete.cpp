#include<bits/stdc++.h>
using namespace std;
void fprint(int n){
    ofstream ofs("output_templete.txt");
    ofs << 1 << ' ' << 1 << ' ' << 0 << '\n';
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            ofs << 0 << " \n"[j==n-1];
        }
    }
}
int main(){
    fprint(6);
}