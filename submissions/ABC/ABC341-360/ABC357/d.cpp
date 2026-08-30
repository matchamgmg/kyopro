#include <bits/stdc++.h>
#include <atcoder/modint>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
using mint = atcoder::modint998244353;

int main(){
    ll N;
    cin >> N;
    ll n = N;
    mint r = 1;
    while(n>0){
        n /= 10;
        r *= mint(10);
    }
    mint ans = mint(N) * (r.pow(N) - mint(1)) / (r - mint(1));
    cout << ans.val() << endl;
    cout << r.pow(N).val() << endl;
}