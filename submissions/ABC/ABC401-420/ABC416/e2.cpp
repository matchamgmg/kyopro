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

    auto floyd_warshall = [&](vector<vector<ll>> &dist){
        int V = dist.size();

        for(int k=0; k<V; ++k){
            for(int i=0; i<V; ++i){
                for(int j=0; j<V; ++j){
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    };

    int N, M;
    cin >> N >> M;
    const ll INF = 1LL<<60;
    vector dist(N+1, vector<ll>(N+1,INF));
    rep(i,0,N){
        dist[i][i] = 0;
    }
    rep(i,0,M){
        int a, b, c;
        cin >> a >> b >> c;
        a--; b--;
        dist[a][b] = min(dist[a][b], (ll)c);
        dist[b][a] = min(dist[b][a], (ll)c);
    }

    int K;
    ll T;
    cin >> K >> T;
    set<int> D;
    int d;
    rep(i,0,K){
        cin >> d;
        d--;
        D.emplace(d);
    }
    dist[N][N] = 0;
    rep(i,0,N){
        if(D.count(i)){
            dist[i][N] = T;
            dist[N][i] = 0;
        }
    }

    floyd_warshall(dist);

    int Q;
    cin >> Q;
    int w, x, y;
    ll t;
    rep(q,0,Q){
        cin >> w;
        if(w==1){
            cin >> x >> y >> t;
            x--; y--;
            dist[x][y] = min(dist[x][y],t);
            dist[y][x] = min(dist[y][x],t);
            rep(i,0,N+1){
                rep(j,0,N+1){
                    dist[i][j] = min(dist[i][j], dist[i][x]+dist[x][y]+dist[y][j]);
                    dist[i][j] = min(dist[i][j], dist[i][y]+dist[y][x]+dist[x][j]);
                }
            }
        }else if(w==2){
            cin >> x;
            x--;
            D.emplace(x);
            dist[x][N] = min(dist[x][N], T);
            dist[N][x] = min(dist[N][x], 0LL);
            rep(i,0,N+1){
                rep(j,0,N+1){
                    dist[i][j] = min(dist[i][j], dist[i][x]+dist[x][N]+dist[N][j]);
                    dist[i][j] = min(dist[i][j], dist[i][N]+dist[N][x]+dist[x][j]);
                }
            }
        }else{
            ll ans = 0;
            rep(i,0,N){
                rep(j,0,N){
                    ll time = INF;
                    time = min(time, dist[i][j]);
                    if(time == INF) time = 0;
                    ans += time;
                }
            }
            cout << ans << endl;
        }
    }
}