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

// 耳DP ? 
// i日目の店舗がD, その時何日連続か 0 : 連続0回, 1 : 1回, 2 : 2回以上
// O(D*N^2)

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, D;
    cin >> N >> D;
    vector<vector<int>> P(D,vector<int>(N));
    rep(i,0,D){
        rep(j,0,N){
            cin >> P[i][j];
        }
    }

    const int INF = 1<<29;
    vector dp(N, vector<int>(3, INF));
    rep(i,0,N){
        dp[i][0] = 0;
    }
    rep(i,0,D){
        vector next_dp(N, vector<int>(3, INF));
        // 前日までの最小価格vが商品jによるものであったとき
        // next_dp[j][1] は正しい値ではないが, つねにnext_dp[j][1] > next_dp[j][2,3]
        // となるためほかの遷移に影響を与えることはない
        int mn = INF;
        rep(j,0,N){
            rep(k,0,3){
                chmin(mn, dp[j][k]);
            }
        }
        // 同じ店のとき
        rep(j,0,N){
            rep(k,0,3){
                int add = P[i][j];
                if(k == 1) add = add * 9 / 10;
                else if(k == 2) add = add * 7 / 10;
                chmin(next_dp[j][min(k+1,2)], dp[j][k] + add);
            }
        }
        // 違う店のとき
        rep(j,0,N){
            chmin(next_dp[j][1], mn + P[i][j]);
        }

        swap(next_dp, dp);
    }

    int ans = INF;
    rep(j,0,N){
        rep(k,0,3){
            chmin(ans, dp[j][k]);
        }
    }
    cout << ans << endl;
}