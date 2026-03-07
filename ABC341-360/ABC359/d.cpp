#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define MOD 998244353

int main(){
    int N,K;
    string S;
    cin >> N >> K >> S;

    auto check = [&](const string &s){
        string rs = s;
        reverse(ALL(rs));
        rep(i,s.size()){
            if(s[i]!=rs[i]){
                return false;
            }
        }
        return true;
    };

    vector<map<string,int>> dp(N+1,map<string,int>());
    dp[0][""] = 1;
    rep(i,N){
        for(auto e : dp[i]){
            string s = e.first;
            if(S[i]=='A'||S[i]=='?'){
                if(i<K-1){
                    dp[i+1][s+'A'] += e.second;
                    dp[i+1][s+'A'] %= MOD;
                }else if(i==K-1){
                    if(!check(s+'A')){
                        dp[i+1][s+'A'] += e.second;
                        dp[i+1][s+'A'] %= MOD;
                    }
                }else{
                    string ss = s.substr(1,K-1);
                    if(!check(ss+'A')){
                        dp[i+1][ss+'A'] += e.second;
                        dp[i+1][ss+'A'] %= MOD;
                    }
                }
            }
            if(S[i]=='B'||S[i]=='?'){
                if(i<K-1){
                    dp[i+1][s+'B'] += e.second;
                    dp[i+1][s+'B'] %= MOD;
                }else if(i==K-1){
                    if(!check(s+'B')){
                        dp[i+1][s+'B'] += e.second;
                        dp[i+1][s+'B'] %= MOD;
                    }
                }else{
                    string ss = s.substr(1,K-1);
                    if(!check(ss+'B')){
                        dp[i+1][ss+'B'] += e.second;
                        dp[i+1][ss+'B'] %= MOD;
                    }
                }
            }
        }
    }
    int ans = 0;
    for(auto e : dp[N]){
        ans += e.second;
        ans %= MOD;
    }
    cout << ans << endl;
}