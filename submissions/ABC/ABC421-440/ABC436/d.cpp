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
    int H, W;
    cin >> H >> W;
    vector<string> S(H);
    rep(i,0,H) cin >> S[i];
    vector warp(26,vector<int>());
    vector<bool> used(26,false);

    rep(i,0,H){
        rep(j,0,W){
            int c = S[i][j]-'a';
            if(0<=c && c<=26){
                warp[c].push_back(i*W+j);
            }
        }
    }


    vector<int> dx ={0,1,0,-1}, dy = {1,0,-1,0};
    vector<int> dist(H*W,-1);
    dist[0] = 0;
    queue<int> que;
    que.push(0);
    int nx, ny;
    while(!que.empty()){
        int u = que.front();
        que.pop();
        rep(i,0,4){
            nx = u/W + dx[i];
            ny = u%W + dy[i];
            if(!grid_check(nx,ny,H,W)) continue;
            if(S[nx][ny] == '#') continue;
            if(dist[nx*W+ny] != -1) continue;
            dist[nx*W+ny] = dist[u] + 1;
            que.push(nx*W+ny);
        }
        int c = S[u/W][u%W]-'a';
        if(0<=c && c <= 26 && used[c]==false){
            for(int v : warp[c]){
                if(dist[v] != -1) continue;
                dist[v] = dist[u] + 1;
                que.push(v);
            }
            used[c] = true;
        }
    }
    cout << dist[H*W-1] << endl;
}