#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
//using mint = modint1000000007;
using vi = vector<int>;
using vll = vector<ll>;
using vld = vector<ld>;
using vmint = vector<mint>;
using vvi = vector<vi>;
using vvll = vector<vll>;
using vvld = vector<vld>;
using vvmint = vector<vmint>;
using vvvi = vector<vvi>;
using vvvll = vector<vvll>;
using vvvld = vector<vvld>;
using vvvmint = vector<vvmint>;
using vb = vector<bool>;
using vvb = vector<vb>;
template<class T> using pq = priority_queue<T, vector<T>>;//大きい順
template<class T> using pq_g = priority_queue<T, vector<T>, greater<T>>;//小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]
template<class T>
bool chmax(T &a, T b){ if(a < b){ a = b; return true; } else return false; }
template<class T>
bool chmin(T &a, T b){ if(a > b){ a = b; return true; } else return false; }

// DPテーブルの復元過程で
// すべての経路がその商品を（買った場合 / 買わなかった場合）を通る
// or どちらもある

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, M;
    cin >> N >> M;
    vll P(N), V(N);
    rep(i,0,N){
        cin >> P[i] >> V[i];
    }

    const ll INF = 1LL<<60;

    vector dp(N+1, vector<ll>(M+1, -INF));
    rep(i,0,N+1){
        rep(j,0,M+1){
            dp[i][j] = -INF;
        }
    }
    dp[0][0] = 0;
    rep(i,0,N){
        rep(j,0,M+1){
            chmax(dp[i+1][j], dp[i][j]);
            if(j - P[i]>=0) chmax(dp[i+1][j], dp[i][j-P[i]] + V[i]);
        }
    }

    // 復元
    ll ans = -INF;
    vector<bool> candi(M+1,false);
    rep(i,0,M+1){
        chmax(ans, dp[N][i]);
    }
    rep(i,0,M+1){
        if(ans == dp[N][i]) candi[i] = true;
    }
    vector<char> Ans(N);
    rrep(i,1,N+1){
        vector<bool> next_candi(M+1,false);
        bool tate = false;
        bool naname = false;
        rep(j,0,M+1){
            if(!candi[j]) continue;
            if(dp[i-1][j] == dp[i][j]){
                next_candi[j] = true;
                tate |= true;
            }
            if(j-P[i-1]>=0 && dp[i-1][j-P[i-1]]+V[i-1] == dp[i][j]){
                next_candi[j-P[i-1]] = true;
                naname |= true;
            }
        }
        if(tate && naname){
            Ans[i-1] = 'B';
        }else if(tate){
            Ans[i-1] = 'C';
        }else{
            Ans[i-1] = 'A';
        }
        swap(candi, next_candi);
    }
    rep(i,0,N){
        cout << Ans[i];
    }
    cout << endl;
}