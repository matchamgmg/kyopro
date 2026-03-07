#include <bits/stdc++.h>
#include <atcoder/all>
#define rep(i, s, n) for (int i = s; i < (n); i++)
#define ALL(a) a.begin(),a.end()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
using namespace std;
using namespace atcoder;
using ll = long long;
using pii = pair<int,int>;
using vi = vector<int>;
using vll = vector<ll>;
using vpii = vector<pii>;
using vvi = vector<vi>;
using vvll = vector<vll>;

int main()
{
    string s,t;
    cin >> s >> t;
    int size_s = s.size();
    int size_t = t.size();
    vvi dp(size_s+1,vi(size_t+1,0));
    //sのi文字目tのj文字目まで見た時の最長共通部分列の長さ
    rep(i,0,size_s){
        rep(j,0,size_t){
            if(s[i]==t[j]){
                dp[i+1][j+1] = dp[i][j]+1;
            }else{
                //s[i],t[j]のどちらかはLCSの最後の文字でない
                //s[i]がLCSの最後の文字でない...dp[i+1][j+1] = dp[i][j+1];
                //t[j]がLCSの最後の文字でない...dp[i+1][j+1] = dp[i+1][j];
                dp[i+1][j+1] = max(dp[i+1][j],dp[i][j+1]);
            }
        }
    }
    //cout << dp[size_s][size_t] << endl;
    //dp配列を逆にたどってLCSを構成
    int size_LCS = dp[size_s][size_t];
    vector<char> ans(size_LCS);
    size_LCS--;
    int i = size_s-1;
    int j = size_t-1;
    while(size_LCS>=0){
        if(s[i]==t[j]){
            ans[size_LCS] = s[i];
            i--;
            j--;
            size_LCS--;
        }else if(dp[i+1][j+1]==dp[i][j+1]){
            i--;
        }else{
            j--;
        }
    }
    rep(i,0,ans.size()){
        cout << ans[i];
    }
    cout << endl;
}