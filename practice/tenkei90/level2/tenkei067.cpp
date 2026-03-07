#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    string N;
    int K;
    cin >> N >> K;
    if(N=="0"){
        cout << N << endl;
        return 0;

    }
    vector<int> base8(25,0),base9(25,0);
    ll n = 0;
    int digit_8,digit_9;
    rep(i,N.size()){
        base8[N.size()-1-i] = N.at(i) - '0';
    }
    digit_8 = N.size();
    rep(i,K){
        rep(j,digit_8){
            ll a = base8[j];
            rep(k,j){
                a *= 8;
            }
            n += a;
        }
        int l = 0;
        while(n>0){
            base9[l] = n%9;
            n /= 9;
            l++;
        }
        digit_9 = l;
        rep(j,digit_9){
            if(base9[j]==8) base9[j] = 5;
            base8[j] = base9[j];
        }
        digit_8 = digit_9;
    }
    rep(i,digit_8){
        cout << base8[digit_8 -1-i];
    }
    cout <<endl;
}