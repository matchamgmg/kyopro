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

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N = 3;
    vector<vector<long long>> A(N, vector<long long>(N));
    rep(i,0,N){
        rep(j,0,N) cin >> A[i][j];
    }

    vector<vector<int>> B(N, vector<int>(N, 0));

    auto win = [N,A](vector<vector<int>> B) -> bool {
        rep(_,0,2){
            rep(i,0,N){
                bool ok = true;
                rep(j,0,N){
                    if(B[i][j] != 1) ok = false;
                }
                if(ok) return true;
            }
            if(B[0][0] == 1 && B[1][1] == 1 && B[2][2] == 1) return true;

            vector<vector<int>> oB = B;
            rep(i,0,N){
                rep(j,0,N){
                    B[i][j] = oB[N-j-1][i];
                }
            }
        }
        return false;
    };

    // その盤面から始めた時に，先手が勝つか(先手1, 後手-1)

    auto DFS = [&](auto f, vector<vector<int>> B) -> bool {
        bool end = true;
        rep(i,0,N){
            rep(j,0,N){
                end &= (B[i][j] != 0);
            }
        }
        if(end){
            ll s = 0;
            rep(i,0,N){
                rep(j,0,N){
                    s += A[i][j]*B[i][j];
                }
            }
            return s > 0;
        }

        rep(i,0,N){
            rep(j,0,N){
                if(B[i][j] == 0){
                    vector<vector<int>> nB = B;
                    nB[i][j] = 1;
                    if(win(nB)) return true;
                    rep(ni,0,N){
                        rep(nj,0,N){
                            nB[ni][nj] = -nB[ni][nj];
                        }
                    }
                    if(!f(f,nB)) return true;
                }
            }
        }
        return false;
    };

    if(DFS(DFS, B)){
        cout << "Takahashi" << endl;
    }else{
        cout << "Aoki" << endl;
    }
}