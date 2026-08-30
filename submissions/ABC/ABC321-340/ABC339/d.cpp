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

// 状態：プレイヤー2人の座標 O(N^4)

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<string> S(N);
    rep(i,0,N) cin >> S[i];

    struct State {
        pair<int,int> a, b;
        State(){}
        State(int x1, int y1, int x2, int y2) : a(x1,y1), b(x2,y2) {}

        constexpr bool operator<(const State &c) const {
            return a == c.a ? b < c.b : a < c.a;
        }
    };

    bool player1 = false;
    int x1, y1, x2, y2;
    rep(i,0,N){
        rep(j,0,N){
            if(S[i][j] == 'P'){
                if(!player1){
                    player1 = true;
                    x1 = i; y1 = j;
                }else{
                    x2 = i; y2 = j;
                }
            }
        }
    }

    const int INF = 1<<29;
    vector dist(N, vector<vector<vector<int>>>(N, vector<vector<int>>(N, vector<int>(N, INF))));
    queue<State> que;
    State st(x1,y1,x2,y2);
    dist[x1][y1][x2][y2] = 0;
    que.emplace(st);

    vector<int> dx = {0,1,0,-1}, dy = {1,0,-1,0};

    while(!que.empty()){
        State s = que.front();
        que.pop();
        int k = dist[s.a.first][s.a.second][s.b.first][s.b.second];
        rep(d,0,4){
            int nx1, ny1, nx2, ny2;
            nx1 = s.a.first + dx[d];
            ny1 = s.a.second + dy[d];
            nx2 = s.b.first + dx[d];
            ny2 = s.b.second + dy[d];
            if(!grid_check(nx1,ny1,N,N)){
                nx1 = s.a.first;
                ny1 = s.a.second;
            }
            else if(S[nx1][ny1] == '#'){
                nx1 = s.a.first;
                ny1 = s.a.second;
            }
            if(!grid_check(nx2,ny2,N,N)){
                nx2 = s.b.first;
                ny2 = s.b.second;
            }
            else if(S[nx2][ny2] == '#'){
                nx2 = s.b.first;
                ny2 = s.b.second;
            }
            if(!(nx1 == nx2 ? ny1 < ny2 : nx1 < nx2)){
                swap(nx1, nx2);
                swap(ny1, ny2);
            }
            State nst(nx1,ny1,nx2,ny2);
            if(dist[nx1][ny1][nx2][ny2] != INF) continue;
            dist[nx1][ny1][nx2][ny2] = k + 1;
            que.emplace(nst);
        }
    }

    int ans = INF;

    rep(i,0,N){
        rep(j,0,N){
            chmin(ans, dist[i][j][i][j]);
        }
    }

    cout << (ans == INF ? -1 : ans) << endl;
}