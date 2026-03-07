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
    vi K(M),C(M);
    vvi A(M,vi());
    rep(i,0,M){
        cin >> K[i] >> C[i];
        vi Ai(K[i]);
        rep(j,0,K[i]){
            cin >> Ai[j];
            Ai[j]--;
        }
        A[i] = Ai;
    }
    dsu uf(N);
    vi idx(M);
    iota(all(idx),0);
    sort(all(idx),[&](int i, int j){ return C[i]<C[j]; });
    ll ans = 0;
    for(int i : idx){
        rep(j,0,K[i]-1){
            int a = A[i][j];
            int b = A[i][j+1];
            if(uf.same(a,b)) continue;
            uf.merge(a,b);
            ans += C[i];
        }
    }
    if(uf.size(0)!=N){
        cout << -1 << endl;
    }else{
        cout << ans << endl;
    }
}