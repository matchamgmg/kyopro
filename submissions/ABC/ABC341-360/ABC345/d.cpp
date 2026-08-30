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

// タイルiを・使う・そのままの向きで使う・回転して使う
// 左上を置ける場所の中で最も初めのもの

// N! * 2^N * HW

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, H, W;
    cin >> N >> H >> W;
    vector<int> A(N), B(N);
    rep(i,0,N) cin >> A[i] >> B[i];

    vector<vector<bool>> board(H, vector<bool>(W, false));
    vector<int> idx(N);
    iota(all(idx),0);

    auto check = [&](int i, int j, int h, int w) -> bool {
        bool res = true;
        if(i+h-1 >= H) return false;
        if(j+w-1 >= W) return false;
        rep(a,i,i+h){
            rep(b,j,j+w){
                res &= (!board[a][b]);
            }
        }
        return res;
    };

    auto cover = [&](int i, int j, int h, int w) -> void {
        rep(a,i,i+h){
            rep(b,j,j+w){
                board[a][b] = true;
            }
        }
        return;
    };

    auto uncover = [&](int i, int j, int h, int w) -> void {
        rep(a,i,i+h){
            rep(b,j,j+w){
                board[a][b] = false;
            }
        }
        return;
    };

    auto DFS = [&](auto f, int d) -> bool {
        if(d == N){
            bool res = true;
            rep(i,0,H){
                rep(j,0,W){
                    res &= board[i][j];
                }
            }

            return res;
        }
        bool res = false;
        // そのまま使う
        int h = -1, w = -1;
        rep(i,0,H){
            rep(j,0,W){
                if(check(i,j,A[idx[d]],B[idx[d]])){
                    h = i;
                    w = j;
                    break;
                }
            }
        }
        if(h!=-1 && w!=-1){
            cover(h,w,A[idx[d]],B[idx[d]]);
            res |= f(f, d+1);
            uncover(h,w,A[idx[d]],B[idx[d]]);
        }
        // 回転して使う
        h = -1, w = -1;
        rep(i,0,H){
            rep(j,0,W){
                if(check(i,j,B[idx[d]],A[idx[d]])){
                    h = i;
                    w = j;
                    break;
                }
            }
        }
        if(h!=-1 && w!=-1){
            cover(h,w,B[idx[d]],A[idx[d]]);
            res |= f(f, d+1);
            uncover(h,w,B[idx[d]],A[idx[d]]);
        }
        if(!res){
            bool ok = true;
            rep(i,0,H){
                rep(j,0,W){
                    ok &= board[i][j];
                }    
            }
            res = ok;
        }
        return res;
    };

    bool ans = false;
    do{
        ans |= DFS(DFS,0);
    }while(next_permutation(all(idx)));

    YN(ans);
}