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
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}

int main()
{
    int N,M;
    cin >> N >> M;
    vi L(N), R(N);
    rep(i,0,N){
        cin >> L[i] >> R[i];
    }
    vi idx(N);
    iota(all(idx),0);
    sort(all(idx),[&](int i, int j){ return R[i] < R[j]; });
    // rを固定したときに(l,r)が条件を満たすような最小のl = d_rを求める
    // このとき，rに対してans = (r-d_r+1)
    //
    vi d_r(M+1,1);
    int idx_R = 0;
    rep(r,1,M+1){
        chmax(d_r[r],d_r[r-1]);
        int max_L = 0;
        while(idx_R<N && r==R[idx[idx_R]]){
            chmax(max_L,L[idx[idx_R]]);
            idx_R++;
        }
        d_r[r] = max(d_r[r-1],max_L+1);
    }
    ll ans = 0;
    rep(i,1,M+1){
        ans += i - d_r[i] + 1;
    }
    cout << ans << endl;
}