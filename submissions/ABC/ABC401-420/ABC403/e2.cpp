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

// 多重集合Xのある要素S_Xについて，S_Xを接頭辞に持つ多重集合Yの要素S_Yの個数
// Yに文字列を追加するとき，+1 or +0
// Xに文字列を追加するとき，+S_Xのノード以降にあるS_Yの個数
// そのS_YがすでにほかのS_Xを接頭辞に持つとき

// trie全体の情報は，根に伝搬させる

struct Node {
    char c;
    bool flag;
    int cnt;
    map<int,int> edges;
    Node(char s) : c(s), flag(false), cnt(0) {}
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Node> nodes;
    int Q;
    cin >> Q;
    int node_num = 0;
    nodes.emplace_back('.');
    node_num++;

    auto f1 = [&](auto f, string &str,int num, int d, bool flag = false) -> int {
        if(d == str.size()){
            flag |= nodes[num].flag;
            if(flag){
                return 0;
            }else{
                nodes[num].flag = true;
                int cnt = nodes[num].cnt;
                nodes[num].cnt -= cnt;
                return cnt;
            }
        }
        int res;
        if(!nodes[num].edges.count(str[d]-'a')){
            nodes.emplace_back(str[d]);
            nodes[num].edges[str[d]-'a'] = node_num;
            node_num++;
        }
        flag |= nodes[num].flag;
        res = f(f,str,nodes[num].edges[str[d]-'a'],d+1,flag);
        nodes[num].cnt -= res;
        return res;
    };

    auto f2 = [&](auto f, string &str, int num, int d, bool flag = false) -> int {
        if(d == str.size()){
            flag |= nodes[num].flag;
            if(flag){
                return 0;
            }else{
                nodes[num].cnt++;
                return 1;
            }
        }
        int res;
        if(!nodes[num].edges.count(str[d]-'a')){
            nodes.emplace_back(str[d]);
            nodes[num].edges[str[d]-'a'] = node_num;
            node_num++;
        }
        flag |= nodes[num].flag;
        res = f(f, str, nodes[num].edges[str[d]-'a'], d+1, flag);
        nodes[num].cnt += res;
        return res;
    };

    int t;
    string S;
    for(;Q--;){
        cin >> t;
        if(t == 1){
            cin >> S;
            f1(f1, S, 0, 0);
        }else{
            cin >> S;
            f2(f2, S, 0, 0);
        }
        cout << nodes[0].cnt << endl;
    }
}