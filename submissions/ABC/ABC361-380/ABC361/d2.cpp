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

// N <= 14
// あり得るパターン
// (N+1)*(N!)/(B!W!) <= 15*14!/(7!7!) = 51480

// 状態を頂点として扱う
// グラフ上の最短経路(BFS)
// 遷移があるごとに次の状態を生成する

int main()
{
    int N;
    cin >> N;
    string S,T;
    cin >> S >> T;
    S += "..";
    T += "..";

    // auto f = [](auto f, int n) -> ll {
    //     if(n==1) return 1;
    //     return n*f(f,n-1);
    // };
    // cout << f(f,15) << endl;
    // cout << f(f,7) << endl;
    // cout << (f(f,15)/(f(f,7)*f(f,7))) << endl;

    map<string,int> dist;
    queue<string> que;
    que.push(S);
    while(!que.empty()){
        string u = que.front();
        que.pop();
        int j = 0;  //..の位置
        while(u[j]!='.') j++;
        // すべての遷移
        rep(i,0,N+1){
            if(u[i]=='.' || u[i+1]=='.') continue;
            string v = u;
            swap(v[j],v[i]);
            swap(v[j+1],v[i+1]);
            if(dist.count(v)) continue;
            dist[v] = dist[u] + 1;
            que.push(v);
        }
    }

    if(dist.count(T)){
        cout << dist[T] << endl;
    }else{
        cout << -1 << endl;
    }
}