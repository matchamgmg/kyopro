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

// 区間について'.'と'x'の個数を前計算しておく

#define INF 1<<29

int main()
{
    int H,W,K;
    cin >> H >> W >> K;
    vector<string> S(H);
    rep(i,0,H){
        cin >> S[i];
    }
    vvi cnt_xw(H,vi(W+1,0)), cnt_dw(H,vi(W+1,0));
    vvi cnt_xh(W,vi(H+1,0)), cnt_dh(W,vi(H+1,0));
    rep(i,0,H){
        rep(j,0,W){
            cnt_xw[i][j+1] = cnt_xw[i][j] + (S[i][j]=='x' ? 1 : 0);
            cnt_dw[i][j+1] = cnt_dw[i][j] + (S[i][j]=='.' ? 1 : 0);
        }
    }
    rep(i,0,W){
        rep(j,0,H){
            cnt_xh[i][j+1] = cnt_xh[i][j] + (S[j][i]=='x' ? 1 : 0);
            cnt_dh[i][j+1] = cnt_dh[i][j] + (S[j][i]=='.' ? 1 : 0);
        }
    }

    int ans = INF;
    rep(i,0,H){
        if(W<K) break;
        rep(j,0,W-K+1){
            if(cnt_xw[i][j+K]-cnt_xw[i][j]==0){
                chmin(ans,cnt_dw[i][j+K]-cnt_dw[i][j]);
            }
        }
    }
    rep(i,0,W){
        if(H<K) break;
        rep(j,0,H-K+1){
            if(cnt_xh[i][j+K]-cnt_xh[i][j]==0){
                chmin(ans,cnt_dh[i][j+K]-cnt_dh[i][j]);
            }
        }
    }
    cout << (ans==INF ? -1 : ans) << endl;
}