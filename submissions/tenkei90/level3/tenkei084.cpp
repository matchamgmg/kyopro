#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    string S;
    cin >> N >> S;
    vector<ll> ans(N,0);
    ll last_o = -1,last_x = -1;
    if(S[0]=='o'){
        last_o = 0;
    }else{
        last_x = 0;
    }
    rep(i,N-1){
        if(S[i+1]=='o'){
            ans[i+1] = ans[i] + last_x+1;
            last_o = i+1;
        }else{
            ans[i+1] = ans[i] + last_o+1;
            last_x = i+1;
        }
    }
    cout << ans[N-1] << endl;
}