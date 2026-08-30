#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int n,l,r;
    cin >> n >> l >> r;
    rep(i,n){
        if(i+1>=l&&i+1<=r){
            cout << r-(i+1)+l << " ";
        }
        else{
            cout << i+1 << " ";
        }
    }
}