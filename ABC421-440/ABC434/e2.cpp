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
    vll X(N), R(N);
    rep(i,0,N){
        cin >> X[i] >> R[i];
    }
    // 木の判定
    // map, dfs
    // 根から辺をたどっていく
    // 部分木が木なら，親ノードにマージしたときも木になる
    // 同じ辺をたどらないように注意 -> 辺番号を一緒に持つ, 来た頂点をもつなど
    map<ll,vector<pair<ll,int>>> G;
    rep(i,0,N){
        ll r = X[i] + R[i];
        ll l = X[i] - R[i];
        G[r].emplace_back(l,i);
        G[l].emplace_back(r,i);
    }

    set<ll> visited;
    auto dfs = [&](auto rc, int c, int edge_num) -> pair<ll,int> {
        visited.insert(c);
        int num = 1;
        int is_tree = true;
        for(auto& [d,e] : G[c]){
            if(e == edge_num) continue;
            if(visited.count(d)){
                is_tree = false;
            }else{
                auto [n,f] = rc(rc,d,e);
                num += n;
                is_tree &= f;
            }
        }
        return make_pair(num, is_tree);
    };

    int ans = 0;
    for(auto& [key,_] : G){
        if(visited.count(key)) continue;
        auto [num,is_tree] = dfs(dfs,key,-1);
        ans += is_tree ? num -1 : num;
    }
    cout << ans << endl;
}