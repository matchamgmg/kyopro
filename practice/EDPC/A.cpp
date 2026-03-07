#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define INF 1<<29

//2^29 = 5e8程度
//chminは危険

template<class T>
void chmin(T &a,T b){
    if(a>b) a = b;
}

int main()
{
    int N;
    cin >> N;
    vector<int> H(N);
    rep(i,N){
        cin >> H[i];
    }
    vector<int> dp(N,INF);//dp[k]...足場kにたどり着くまでのコストの総和の最小値 0<k<N-1
    dp[0] = 0;
    rep(i,N-1){
        //i->i+1
        chmin(dp[i+1],dp[i]+abs(H[i+1]-H[i]));
        //i->i+2
        if(i+2<N){
            chmin(dp[i+2],dp[i]+abs(H[i+2]-H[i]));
        }
    }
    cout << dp[N-1] << endl;
}