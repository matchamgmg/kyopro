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
    ll N, Q, S, T;
    cin >> N >> Q >> S >> T;
    vll A(N+1);
    rep(i,0,N+1){
        cin >> A[i];
    }
    vll D(N);
    rep(i,0,N){
        D[i] = A[i+1] - A[i];
    }
    ll degree = 0;
    rep(i,0,N){
        degree -= D[i] * (D[i] > 0 ? S : T);
    }
    while(Q--){
        ll L, R, X;
        cin >> L >> R >> X;
        L--;
        ll before = 0, after = 0;
        before -= D[L] * (D[L] > 0 ? S : T);
        if(R<N) before -= D[R] * (D[R] > 0 ? S : T);
        D[L] += X;
        if(R<N) D[R] -= X;
        after -= D[L] * (D[L] > 0 ? S : T);
        if(R<N) after -= D[R] * (D[R] > 0 ? S : T);
        degree = degree - before + after;
        cout << degree << endl;
    }
}