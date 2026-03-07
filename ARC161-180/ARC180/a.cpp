#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define MOD 1'000'000'007

int main()
{
    int N;
    string S;
    cin >> N >> S;
    ll ans = 1;
    ll cnt = 1;
    ll memo = 0;
    for(int i=1;i<N;i++){
        if(S[i]!=S[i-1]){
            cnt++;
            if(i==N-1){
                memo = cnt;
            }
        }else{
            memo = cnt;
            cnt = 1;
        }
        if(memo<3) continue;
        /*string s = "";
        rep(j,memo){
            if(i==N-1){
                s += S[i-memo+j+1];
            }else{
                s += S[i-memo+j];
            }
        }*/
        //cout << "found " << s << " memo = " << memo << endl;
        ll prod = (memo + 1)/2;
        ans = ans * prod % MOD;
        //cout << prod << endl;
        //cout << ans << endl;
        memo = 0;
    }
    cout << ans << endl;
}