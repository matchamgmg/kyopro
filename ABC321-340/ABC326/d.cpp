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

// 25 マス
// 各行について文字の書きこみ方は NC3 * 3! = 60
// 60^5 = 3600^2 * 60 = 216 * 10^5 = 2 * 10^7

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    string R, C;
    cin >> N >> R >> C;

    vector<vector<char>> Board(N, vector<char>(N,'.'));
    vector<int> rowABC(N,0), colABC(N,0); ///000 ~ 111

    vector<vector<char>> ans(N, vector<char>(N, '.'));

    auto DFS1 = [&](auto f, int r_) -> bool {
        bool resf = false;
        if(r_ == N){
            bool ok = true;
            rep(i,0,N){
                rep(j,0,N){
                    if(Board[i][j] != '.'){
                        ok &= (Board[i][j] == R[i]);
                        break;
                    }
                }
            }
            rep(j,0,N){
                rep(i,0,N){
                    if(Board[i][j] != '.'){
                        ok &= (Board[i][j] == C[j]);
                        break;
                    }
                }
            }
            if(ok){
                rep(i,0,N){
                    rep(j,0,N){
                        ans[i][j] = Board[i][j];
                    }
                }
            }
            return ok;
        }
        auto DFS2 = [&](auto g, int r, int c) -> bool {
            bool resg = false;
            if(c == N){
                resg |= f(f, r+1);
                return resg;
            }
            if((~rowABC[r]&1) && (~colABC[c]&1)){
                Board[r][c] = 'A';
                rowABC[r] += 1;
                colABC[c] += 1;
                resg |= g(g, r, c+1);
                Board[r][c] = '.';
                rowABC[r] -= 1;
                colABC[c] -= 1;
            }
            if((~rowABC[r]>>1&1) && (~colABC[c]>>1&1)){
                Board[r][c] = 'B';
                rowABC[r] += 2;
                colABC[c] += 2;
                resg |= g(g, r, c+1);
                Board[r][c] = '.';
                rowABC[r] -= 2;
                colABC[c] -= 2;
            }
            if((~rowABC[r]>>2&1) && (~colABC[c]>>2&1)){
                Board[r][c] = 'C';
                rowABC[r] += 4;
                colABC[c] += 4;
                resg |= g(g, r, c+1);
                Board[r][c] = '.';
                rowABC[r] -= 4;
                colABC[c] -= 4;
            }
            if((N-c) > ((~rowABC[r]&1) + (~rowABC[r]>>1&1) + (~rowABC[r]>>2&1)) && (N-r) > ((~colABC[c]&1) + (~colABC[c]>>1&1) + (~colABC[c]>>2&1))){
                resg |= g(g, r, c+1);
            }

            return resg;
        };

        resf |= DFS2(DFS2, r_, 0);
        return resf;
    };

    if(DFS1(DFS1, 0)){
        cout << "Yes" << endl;
        rep(i,0,N){
            rep(j,0,N){
                cout << ans[i][j];
            }
            cout << endl;
        }
    }else{
        cout << "No" << endl;
    }
}