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

// 現在の正解している問題，残り所持金
// 提出の結果を見てから次の提出を決められる
// 残り金額が少ないところから計算する

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int INF = 1<<29;
    int N, X;
    cin >> N >> X;
    vector<int> S(N), C(N);
    vector<double> P(N);
    int min_cost = INF;
    rep(i,0,N){
        int p;
        cin >> S[i] >> C[i] >> p;
        P[i] = p/100.;
    }
    int n = 1<<N;

    // 所持金x円，今まで集合acを正解している状態から始めた時の，以降に得られる得点の期待値の最大値
    vector dp(X+1, vector<double>(n, 0.));
    rep(x,0,X+1){
        rep(ac,0,n){
            rep(i,0,N){
                if(ac>>i&1) continue;
                if(C[i]>x) continue;
                double e = dp[x-C[i]][ac]*(1-P[i]) + (dp[x-C[i]][(ac|1<<i)]+S[i])*P[i];
                chmax(dp[x][ac], e);
            }
        }
    }
    
    printf("%.10lf\n",dp[X][0]);
}