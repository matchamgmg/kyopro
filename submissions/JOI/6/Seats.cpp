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
    int N;
    cin >> N;
    vll C(N), X(N);
    rep(i,0,N) cin >> C[i] >> X[i];
    vi idx(N);
    iota(all(idx),0);
    sort(all(idx),[&](int i, int j){ return X[i]<X[j]; });
    int l = 0, r = 0;
    const ll INF = 1LL<<60;
    vll min_dist(N,INF);
    while(r<N){
        while(r<N && C[idx[r]]==C[idx[l]]) r++;
        if(r<N){
            rep(i,l,r){
                chmin(min_dist[idx[i]], X[idx[r]] - X[idx[i]]);
            }
            l = r;
        }
    }
    l = N-1, r = N-1;
    while(l>=0){
        while(l>=0 && C[idx[r]]==C[idx[l]]) l--;
        if(l>=0){
            rrep(i,l+1,r+1){
                chmin(min_dist[idx[i]], X[idx[i]] - X[idx[l]]);
            }
            r = l;
        }
    }
    rep(i,0,N){
        cout << min_dist[i] << endl;
    }
}