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

void solve(){
    int H, W;
    cin >> H >> W;
    vector<string> S(H);
    rep(i,0,H) cin >> S[i];
    vector<int> F(H);
    rep(i,0,H){
        int d = 0;
        rep(j,0,W){
            if(S[i][j]=='#'){
                d += 1;
            }
            d <<= 1;
        }
        F[i] = (d >> 1);
    }
    auto check = [&](int s, int t){
        bool res = false;
        bool black = false;
        rep(i,0,W){
            if((s & t) & 1){
                if(!black) black = true;
                else{
                    res = true;
                }
            }else{
                black = false;
            }
            s >>= 1;
            t >>= 1;
        }
        return !res;
    };
    auto change_count = [&](int s, int t){
        int res = 0;
        rep(i,0,W){
            if((s & 1) && !(t & 1)) res++;
            else if(!(s & 1) && (t & 1)){
                res = -1;
                return res;
            }
            s >>= 1;
            t >>= 1;
        }
        return res;
    };
    int siz = 1<<W;
    const int INF = 1<<29;
    vector dp(H,vector<int>(siz,INF));
    rep(i,0,siz){
        int c = change_count(F[0],i);
        if(c==-1) continue;
        dp[0][i] = c;
    }
    rep(i,0,H-1){
        rep(j,0,siz){
            if(dp[i][j] == INF) continue;
            rep(k,0,siz){
                int c = change_count(F[i+1],k);
                if(c == -1) continue;
                if(!check(j,k)) continue;
                chmin(dp[i+1][k], dp[i][j] + c);
            }
        }
    }
    cout << *min_element(all(dp[H-1])) << endl;
}

int main()
{
    int T;
    cin >> T;
    while(T--) solve();
}