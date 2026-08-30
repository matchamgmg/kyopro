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

// 行列累乗?
// A*A : O(N^3)
const ll INF = 9e18;

vector<vector<ll>> f(vector<vector<ll>> &A,int N){
    vector<vector<ll>> res(N, vector<ll>(N,INF));
    rep(i,0,N){
        rep(j,0,N){
            rep(k,0,N){
                chmin(res[i][j], A[i][k]+A[k][j]);
            }
        }
    }
    return res;
}

vector<vector<ll>> g(vector<vector<ll>> &A1, vector<vector<ll>> &A2, int N){
    vector<vector<ll>> res(N, vector<ll>(N,INF));
    rep(i,0,N){
        rep(j,0,N){
            rep(k,0,N){
                chmin(res[i][j], A1[i][k]+A2[k][j]);
            }
        }
    }
    return res;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, K;
    cin >> N >> K;
    vector<vector<vector<ll>>> dp(1, vector<vector<ll>>(N, vector<ll>(N)));
    rep(i,0,N){
        rep(j,0,N){
            cin >> dp[0][i][j];
        }
    }
    ll k = 0;
    while((1LL<<k) < K){
        dp.emplace_back(f(dp[k], N));
        k++;
    }
    vector<vector<ll>> ans(N, vector<ll>(N, 0));
    vector<int> s;
    rep(l,0,k){
        if(K>>l&1){
            s.emplace_back(l);
        }
    }
    ans = dp[s[0]];
    rep(i,1,s.size()){
        ans = g(ans, dp[s[i]], N);
    }
    rep(i,0,N){
        cout << ans[i][i] << endl;
    }
}