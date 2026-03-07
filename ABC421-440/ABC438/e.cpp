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
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]

int main()
{
    ll K = 0;
    ll n = 1e9+1LL;
    while(n>0){
        K++;
        n >>= 1;
    }
    int N, Q;
    cin >> N >> Q;
    vll A(N);
    rep(i,0,N){
        cin >> A[i];
        A[i]--;
    }
    // V[i][j] i回操作を行ったときにひとjの持っているバケツの水がどれだけ増えるか
    vector V(K,vector<ll>(N,0));
    vector P(K,vector<int>(N,0));
    rep(i,0,N){
        P[0][i] = A[i];
        V[0][i] = i+1;
    }
    rep(i,0,K-1){
        rep(j,0,N){
            P[i+1][j] = P[i][P[i][j]];
        }
    }
    rep(i,0,K-1){
        rep(j,0,N){
            V[i+1][j] = V[i][P[i][j]] + V[i][j];
        }
    }
    // vv_cout(P);
    // cout << endl;
    // vv_cout(V);
    // cout << endl;
    while(Q--){
        ll T, B;
        cin >> T >> B;
        B--;
        ll ans = 0;
        ll t = 0;
        ll p = B;
        while(T > 0){
            if(T & 1){
                ans += V[t][p];
                p = P[t][p];
            }
            T >>= 1;
            t++;
        }
        cout << ans << endl;
    }
}