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

int op1(int a, int b){
    return max(a,b);
}
int op2(int a, int b){
    return min(a,b);
}
int e1(){ return INT32_MIN; }
int e2(){ return INT32_MAX; }

int main()
{
    int N,K;
    cin >> N >> K;
    vi P(N);
    rep(i,0,N){
        cin >> P[i];
        P[i]--;
    }
    vi pos(N);
    rep(i,0,N){
        pos[P[i]] = i;
    }
    /* segtree */
    // segtree<int,op1,e1> max_tree(pos);
    // segtree<int,op2,e2> min_tree(pos);
    // int ans = INT32_MAX;
    // rep(i,0,N-K+1){
    //     chmin(ans,max_tree.prod(i,i+K)-min_tree.prod(i,i+K));
    // }
    /* set::min/max */
    set<int> S;
    rep(i,0,K){
        S.insert(pos[i]);
    }
    int ans = INT32_MAX;
    chmin(ans,*S.rbegin()-*S.begin());
    rep(i,K,N){
        S.erase(pos[i-K]);
        S.insert(pos[i]);
        chmin(ans,*S.rbegin()-*S.begin());
    }
    cout << ans << endl;
}