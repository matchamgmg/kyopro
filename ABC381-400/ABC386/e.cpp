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
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

// 探索の途中で分岐がない部分を省略することで
// 葉以外のすべての頂点について出次数が2となる
// このような頂点を一つ見るごとに子ノードをマージすると考えると
// マージする回数は葉ノードの数-1
// 全体の辺の本数はちょうど葉ノードの数の2倍
// 葉ノードの数 = nCk
// この工夫をしない場合O(N^2)

int main()
{
    int N,K;
    cin >> N >> K;
    vll A(N);
    rep(i,0,N){
        cin >> A[i];
    }
    ll all_xor = 0;
    bool large_k = false;
    if(N-K < K){
        large_k = true;
        K = N-K;
    }
    rep(i,0,N){
        all_xor ^= A[i];
    }
    ll ans = 0;
    auto dfs = [&](auto f, int k, int n, ll x) -> void {
        if(n==N || k==K){
            if(large_k){
                chmax(ans,all_xor^x);
            }else{
                chmax(ans,x);
            }
            return;
        }
        rep(i,0,2){
            if(i==0){
                if(N-n-1 >= K-k){
                    f(f,k,n+1,x);
                }
            }else{
                if(k<K){
                    f(f,k+1,n+1,x^A[n]);
                }
            }
        }
        return;
    };

    dfs(dfs,0,0,0);
    cout << ans << endl;
}