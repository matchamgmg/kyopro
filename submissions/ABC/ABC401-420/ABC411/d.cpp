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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    struct Node {
        int num;
        string s;
        Node(int i, string str) : num(i), s(str) {}
    };

    int N, Q;
    cin >> N >> Q;

    vector<Node> nodes;
    vector<vector<int>> G;
    vector<int> pos(N+1,0);
    int num = 0;
    nodes.emplace_back(num,"");
    G.emplace_back(vector<int>());
    num++;

    while(Q--){
        int t, p;
        string s;
        cin >> t >> p;
        if(t==1){
            pos[p] = pos[0];
        }else if(t==2){
            cin >> s;
            nodes.emplace_back(num,s);
            G.emplace_back(vector<int>());
            int before = pos[p];
            pos[p] = num;
            G[before].emplace_back(num);
            num++;
        }else{
            pos[0] = pos[p];
        }
    }

    string ans = "";
    stack<int> st;

    auto DFS = [&](auto f, int s) -> bool {
        if(pos[0] == s){
            st.push(s);
            return true;
        }
        bool res = false;
        for(int t : G[s]){
            res |= f(f, t);
        }
        if(res){
            st.push(s);
            return true;
        }
        return false;
    };

    DFS(DFS, 0);
    while(!st.empty()){
        int t = st.top();
        st.pop();
        ans += nodes[t].s;
    }

    cout << ans << endl;
}