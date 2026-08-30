#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int a,b;
    cin >>a>>b;
    if(a==b){
        cout << -1 << endl;
    }else if(a*b==2){
        cout << 3 << endl;
    }else if(a*b==3){
        cout << 2 << endl;
    }else{
        cout << 1 << endl;
    }
}