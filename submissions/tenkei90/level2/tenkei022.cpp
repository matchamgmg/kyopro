#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

ll gcd(ll x,ll y){
    if(x>y){
        return x%y?gcd(x%y,y):y;
    }else{
        return y%x?gcd(x,y%x):x;
    }
}

int main()
{
    ll A,B,C;
    cin >> A >> B >> C;
    ll gcd_abc = gcd(gcd(A,B),C);
    cout << (A/gcd_abc - 1)+(B/gcd_abc - 1)+(C/gcd_abc - 1) << endl;
}

//最大公約数...共通の素因数を最大限選ぶ
//3数の場合も同じ