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
    vll A(N+1), B(N);
    rep(i,0,N+1) cin >> A[i];
    rep(i,0,N) cin >> B[i];
    vll idx(N+1);
    iota(all(idx),0);
    sort(all(idx),[&](int i, int j){ return A[i]<A[j]; });
    sort(all(A)); sort(all(B));
    vll O1(N), O2(N);
    rep(i,0,N){
        O1[i] = max(0LL,A[i]-B[i]);
    }
    rep(i,0,N){
        O2[i] = max(0LL,A[i+1]-B[i]);
    }
    vll SO1(N+1,0), SO2(N+1,0);
    rep(i,0,N){
        SO1[i+1] = max(SO1[i], O1[i]);
        SO2[N-i-1] = max(SO2[N-i], O2[N-i-1]);
    }
    vll ans(N+1);
    rep(i,0,N+1){
        ans[idx[i]] = max(SO1[i], SO2[i]);
    }
    v_cout(ans);
    ////////////////////////////////
    // const ll INF = 1LL<<60;
    // vll Ans(N+1, INF);
    // do{
    //     rep(i,0,N+1){
    //         ll c = 0;
    //         rep(j,0,N){
    //             chmax(c, max(A[j+(j>=i)]-B[j], 0LL));
    //         }
    //         chmin(Ans[i], c);
    //     }
    // }while(next_permutation(all(B)));
    // rep(i,0,N+1){
    //     cout << Ans[i] << ' ';
    // }
    // cout << endl;
}