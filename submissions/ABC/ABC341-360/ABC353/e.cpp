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
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]
template<class T>
bool chmax(T &a, T b){ if(a < b){ a = b; return true; } else return false; }
template<class T>
bool chmin(T &a, T b){ if(a > b){ a = b; return true; } else return false; }

// Trie

struct Node {
    int num;
    char c;
    int cnt;
    vector<int> edge;
    Node(){}
    Node(int n, char s) : num(n), c(s), cnt(0) {
        edge.assign(26, -1);
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Node> nodes;
    int node_num = 0;
    Node node(node_num, '.');
    nodes.emplace_back(node);
    node_num++;

    ll ans = 0;

    auto put_string = [&](auto f, string &str, int d, int n) -> void {
        if(d == str.size()){
            ans += nodes[n].cnt;
            nodes[n].cnt++;
            return;
        }
        if(nodes[n].edge[str[d]-'a'] == -1){
            Node node(node_num, str[d]);
            nodes.emplace_back(node);
            nodes[n].edge[str[d]-'a'] = node_num;
            node_num++;
        }
        if(n != 0) ans += nodes[n].cnt;
        nodes[n].cnt++;
        f(f, str, d+1, nodes[n].edge[str[d]-'a']);
    };

    int N;
    cin >> N;
    rep(i,0,N){
        string S;
        cin >> S;
        put_string(put_string, S, 0, 0);
    }
    cout << ans << endl;
}