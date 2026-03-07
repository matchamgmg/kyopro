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

// (0,0,0,0) -> (2,2,2,2) : 3^4 通り
// H = 2 のとき, 縦の二色の累積和をとるとO(3^2 * W)
// H <= 400 のとき, O(H^2 * W)

// 二か所の色のパターン
// (0,0) (0,1) (0,2) (1,1) (1,2) (2,2)

// 二色以下 : (0,3,5) && (0,3,5)
// 三色 : 0 && 4, 3 && 2, 5 && 1, (1,2,4 のうち相異なる二つ)

// ------------------------------------------
// 両隣二頂点と自身の三頂点の色がすべてことなるような頂点Xがちょうど二つある


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int H, W;
    cin >> H >> W;
    vector<vector<int>> A(H,vector<int>(W));
    rep(i,0,H){
        rep(j,0,W) cin >> A[i][j];
    }

    // auto get_color_id = [](int a, int b) -> int {
    //     if(a > b) swap(a, b);
    //     if(a == b){
    //         return (a == 0 ? 0 : a == 1 ? 3 : 5);
    //     }else{
    //         return (a == 1 ? 4 : b == 2 ? 2 : 1);
    //     }
    // };

    // auto have_3_colors = [](int a, int b) -> bool {
    //     if(a > b) swap(a, b);
    //     if(a==0 && b==4 || a==2 && b==3 || a==1 && b==5 || a==1 && b==2 || a==1 && b==4 || a==2 && b==4){
    //         return true;
    //     }else{
    //         return false;
    //     }
    // };

    // auto f = [&](int i1, int i2) -> ll {
    //     ll res = 0;
    //     vector<vector<ll>> Scolor(W+1,vector<ll>(6,0));
    //     rep(j,0,W){
    //         int c1 = A[i1][j], c2 = A[i2][j];
    //         int cid = get_color_id(c1, c2);
    //         rep(k,0,6){
    //             Scolor[j+1][k] = Scolor[j][k] + (cid == k);
    //             if(have_3_colors(k,cid)){
    //                 res += Scolor[j][k];
    //             }
    //         }
    //     }
    //     return res;
    // };

    // ll ans = 0;

    // rep(i,0,H-1){
    //     rep(j,i+1,H){
    //         ans += f(i,j);
    //     }
    // }

    // cout << ans << endl;

    vector tate(3, vector<vector<ll>>(W, vector<ll>(H+1, 0)));
    vector yoko(3, vector<vector<ll>>(H, vector<ll>(W+1, 0)));
    rep(c,0,3){
        rep(i,0,H){
            rep(j,0,W){
                yoko[c][i][j+1] = yoko[c][i][j] + (A[i][j] == c);
            }
        }
    }
    rep(c,0,3){
        rep(i,0,W){
            rep(j,0,H){
                tate[c][i][j+1] = tate[c][i][j] + (A[j][i] == c);
            }
        }
    }

    ll ans = 0;
    rep(i,0,H){
        rep(j,0,W){
            int c = A[i][j];
            ans += (yoko[(c+1)%3][i][W] - yoko[(c+1)%3][i][j+1]) * (tate[(c+2)%3][j][H] - tate[(c+2)%3][j][i+1]);
            ans += (yoko[(c+1)%3][i][W] - yoko[(c+1)%3][i][j+1]) * tate[(c+2)%3][j][i];
            ans += yoko[(c+1)%3][i][j] * (tate[(c+2)%3][j][H] - tate[(c+2)%3][j][i+1]);
            ans += yoko[(c+1)%3][i][j] * tate[(c+2)%3][j][i];

            ans += (yoko[(c+2)%3][i][W] - yoko[(c+2)%3][i][j+1]) * (tate[(c+1)%3][j][H] - tate[(c+1)%3][j][i+1]);
            ans += (yoko[(c+2)%3][i][W] - yoko[(c+2)%3][i][j+1]) * tate[(c+1)%3][j][i];
            ans += yoko[(c+2)%3][i][j] * (tate[(c+1)%3][j][H] - tate[(c+1)%3][j][i+1]);
            ans += yoko[(c+2)%3][i][j] * tate[(c+1)%3][j][i];
        }
    }

    cout << ans / 2 << endl;
}