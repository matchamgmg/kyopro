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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, M;
    cin >> N >> M;
    vi X(M);
    rep(i,0,M) cin >> X[i];
    vector<ll> C(N,0), L(N,0); // imos
    ll before = 0;
    rep(i,0,M-1){
        ll x = X[i], y = X[i+1];
        x--; y--;
        if(x<y) swap(x,y);
        ll dist = min(x-y, N+y-x);
        if(y+dist == x){
            C[y]++;
            if(x<N-1) C[x]--;
            L[y] += dist;
            if(x<N-1) L[x] -= dist;
        }else{
            C[x]++;
            C[0]++;
            C[y]--;
            L[x] += dist;
            L[0] += dist;
            L[y] -= dist;
        }
        before += dist;
    }
    rep(i,0,N-1){
        C[i+1] += C[i];
        L[i+1] += L[i];
    }
    const ll INF = 1LL<<60;
    ll ans = INF;
    rep(i,0,N){
        ll p = L[i], n = N*C[i] - L[i];
        ll after = before - p + n;
        chmin(ans, after);
    }
    cout << ans << endl;
}