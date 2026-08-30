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

// 3! * (4*16)^3
// 3! * 2^24

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N = 3;

    struct Polyomino {
        vector<vector<bool>> mino;
        int u, d, l, r;
        Polyomino() : u(3), d(0), l(3), r(0) {
            mino.assign(4, vector<bool>(4, false));
        }
        Polyomino(vector<string> &S) : u(3), d(0), l(3), r(0) {
            mino.assign(4,vector<bool>(4,false));
            rep(i,0,4){
                rep(j,0,4){
                    if(S[i][j] == '#'){
                        mino[i][j] = true;
                        chmin(u, i);
                        chmax(d, i);
                        chmin(l, j);
                        chmax(r, j);
                    }
                }
            }
        }
        Polyomino right(){
            Polyomino res;
            rep(i,0,4){
                rep(j,0,4){
                    res.mino[i][j] = mino[j][4-i-1];
                }
            }
            rep(i,0,4){
                rep(j,0,4){
                    if(res.mino[i][j]){
                        chmin(res.u, i);
                        chmax(res.d, i);
                        chmin(res.l, j);
                        chmax(res.r, j);
                    }
                }
            }
            return res;
        }
    };
    vector<vector<Polyomino>> P(N, vector<Polyomino>(4));
    int cnt = 0;
    rep(i,0,N){
        vector<string> S(4);
        rep(j,0,4){
            cin >> S[j];
            rep(k,0,4){
                if(S[j][k] == '#') cnt++;
            }
        }
        P[i][0] = Polyomino(S);
        rep(j,1,4){
            P[i][j] = P[i][j-1].right();
        }
    }

    // rep(i,0,3){
    //     rep(j,0,4){
    //         rep(k,0,4){
    //             rep(l,0,4){
    //                 cout << P[i][j].mino[k][l];
    //             }
    //             cout << endl;
    //         }
    //         cout << endl;
    //     }
    // }

    if(cnt != 16){
        cout << "No" << endl;
        return 0;
    }

    bool ans = false;
    vector<vector<bool>> B(4, vector<bool>(4, false));

    auto check = [&](int d, int k, int i, int j) -> bool {
        bool res = true;
        Polyomino &p = P[d][k];
        rep(x,0,4){
            rep(y,0,4){
                if(!grid_check(x-i,y-j,4,4)) continue;
                if(B[x][y] && p.mino[x-i][y-j]) res = false;
            }
        }
        return res;
    };

    auto g = [&](int d, int k, int i, int j) -> void {
        rep(x,0,4){
            rep(y,0,4){
                if(!grid_check(x-i,y-j,4,4)) continue;
                B[x][y] = B[x][y] ^ P[d][k].mino[x-i][y-j];
            }
        }
    };

    auto DFS = [&](auto f, int depth) -> void {
        if(depth == 3){
            ans = true;
            return;
        }
        rep(k,0,4){
            Polyomino &p = P[depth][k];
            rep(i,-3,4){
                rep(j,-3,4){
                    int u = p.u + i;
                    int d = p.d + i;
                    int l = p.l + j;
                    int r = p.r + j;
                    if(!grid_check(u,l,4,4)) continue;
                    if(!grid_check(d,r,4,4)) continue;
                    if(!check(depth,k,i,j)) continue;
                    g(depth,k,i,j);
                    f(f, depth+1);
                    g(depth,k,i,j);
                }
            }
        }
    };

    DFS(DFS, 0);
    YN(ans);
}