#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int a,b;
    cin >> a >> b;
    if(a==b){
        cout << 1 << endl;
    }else if((a-b)%2==0){
        cout << 3 << endl;
    }else{
        cout << 2 << endl;
    }
}