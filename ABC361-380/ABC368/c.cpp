#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<ll> H(N);
    rep(i,N){
        cin >> H[i];
    }
    ll T = 0;
    rep(i,N){
        ll t = T%3;

        ll hp = H[i];
        ll q = hp%5;
        ll p = (hp-q)/5;
        T += 3*p;
        if(q==0){

        }else if((q==1)||(t==2&&q<=3)){
            T += 1;
        }else if((q==2)||(t>=1&&q<=4)){
            T += 2;
        }else{
            T += 3;
        }
    }
    cout << T << endl;
}