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

// Functional Graph : 連結成分ごとに閉路が一つ
// 閉路検出 : その閉路に属する頂点を記録
// 頂点uが閉路の中にあるとき : uから到達できる頂点 -> サイクルのサイズ
// 閉路の外にあるとき : 閉路までのパス長 + サイクルのサイズ

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> F(N);
    rep(i,0,N){
        cin >> F[i];
        F[i]--;
    }

    vector<bool> seen(N, false), finished(N, false);
    stack<int> history;
    vector<int> ans(N, 0);

    auto DFS = [&](auto f, int u) -> int {
        seen[u] = true;
        history.push(u);
        int v = F[u];
        if(seen[v]){
            history.push(v);
            return v;
        }
        int pos = f(f, v);
        return pos;
    };

    auto cycle = [&](stack<int> &hist) -> void {
        stack<int> cyc;
        int s = hist.top();
        cyc.push(s);
        hist.pop();
        while(!hist.empty() && hist.top() != s){
            cyc.push(hist.top());
            hist.pop();
        }
        if(hist.empty()) return;

        while(!hist.empty()) hist.pop();
        int siz = cyc.size();
        while(!cyc.empty()){
            s = cyc.top();
            cyc.pop();
            finished[s] = true;
            ans[s] = siz;
        }
    };

    auto DFS2 = [&](auto f, int u) -> int {
        if(finished[u]) return ans[u];
        finished[u] = true;
        int v = F[u];
        int res = f(f, v) + 1;
        ans[u] = res;
        return res;
    };

    rep(i,0,N){
        if(seen[i]) continue;
        DFS(DFS, i);
        cycle(history);
    }

    rep(i,0,N){
        DFS2(DFS2, i);
    }

    ll result = 0;
    rep(i,0,N){
        result += ans[i];
    }
    cout << result << endl;
}