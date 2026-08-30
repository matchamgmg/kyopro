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

vector<int> dx = {0,1,0,-1}, dy = {1,0,-1,0};
const int INF = 1e9;

int main()
{
    int T;
    cin >> T;
    // 光線の長さは高々2*HW
    while(T--){
        int H,W;
        cin >> H >> W;
        vector<string> S(H);
        rep(i,0,H) cin >> S[i];
        //01BFS
        vector<vector<vector<int>>> cost(H,vector<vector<int>>(W,vector<int>(4,INF)));
        cost[0][0][0] = 0;
        deque<pair<pair<int,int>,int>> que;   // v,direction
        que.push_back({{0,0},0});
        while(!que.empty()){
            auto p = que.front();
            que.pop_front();
            auto u = p.first;
            int d = p.second;
            // 直進
            rep(i,0,4){
                if((i-d+4)%4==2) continue;
                int nx = u.first + dx[i];
                int ny = u.second + dy[i];
                if(grid_check(nx,ny,H,W)){
                    int c = 0;
                    if(i==d && S[u.first][u.second]=='A'){
                        c = 0;
                    }else if(d%2==0 && (i-d+4)%4==1 && S[u.first][u.second]=='B'){
                        c = 0;
                    }else if(d%2==0 && (i-d+4)%4==3 && S[u.first][u.second]=='C'){
                        c = 0;
                    }else if(d%2==1 && (i-d+4)%4==1 && S[u.first][u.second]=='C'){
                        c = 0;
                    }else if(d%2==1 && (i-d+4)%4==3 && S[u.first][u.second]=='B'){
                        c = 0;
                    }else{
                        c = 1;
                    }
                    if(cost[nx][ny][i]<=cost[u.first][u.second][d]+c) continue;
                    chmin(cost[nx][ny][i],cost[u.first][u.second][d]+c);
                    if(c==0){
                        que.push_front({{nx,ny},i});
                    }else{
                        que.push_back({{nx,ny},i});
                    }
                }
            }
        }
        int ans = INF;
        rep(i,0,2){
            int c = 0;
            if(S[H-1][W-1]=='A'){
                if(i==0){
                    c = 0;
                }else{
                    c = 1;
                }
            }else if(S[H-1][W-1]=='B'){
                if(i==1){
                    c = 0;
                }else{
                    c = 1;
                }
            }else{
                c = 1;
            }
            chmin(ans,cost[H-1][W-1][i]+c);
        }
        cout << ans << endl;
    }
}