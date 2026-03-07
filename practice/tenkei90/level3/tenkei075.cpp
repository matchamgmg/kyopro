#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    ll N;
    cin >> N;
    ll cnt = 0;
    //素因数の個数を数える
    /*for(ll p = 2;p*p<=N;p++){
        while(N>0 && N%p==0){
            cnt++;
            N /= p;
        }
    }
    if(N>1) cnt++;
    */
    //素因数分解
    ll rem = N;
    vector<ll> prime;
    for(ll p = 2;p*p<=N;p++){
        while(rem>0 && rem%p==0){
            prime.push_back(p);
            rem /= p;
        }
    }
    if(rem!=1LL) prime.push_back(rem);
    ll n = 0;
    while((1<<n)<cnt) n++;
    cout << n << endl;
}