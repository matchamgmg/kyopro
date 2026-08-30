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
    vector<map<int,int>> Trie(N+1);
    vector<vector<int>> vs(N+1,vector<int>());  // 同じノードを共有している数列を管理
    vector<int> pos(N+1,-1);    // 数列Ai -> node
    pos[0] = 0;
    int node = 1;
    rep(i,1,N+1){
        int p, y;
        cin >> p >> y;
        if(!Trie[pos[p]].count(y)){
            Trie[pos[p]][y] = node;
            node++;
        }
        pos[i] = Trie[pos[p]][y];
        vs[pos[i]].emplace_back(i);
    }

    vector<int> ans;

    auto dfs = [&](auto f, int u) -> void {
        sort(all(vs[u]));
        for(int i : vs[u]){
            ans.emplace_back(i);
        }
        for(auto p : Trie[u]){
            f(f,p.second);
        }
    };

    dfs(dfs,0);

    v_cout(ans);
}