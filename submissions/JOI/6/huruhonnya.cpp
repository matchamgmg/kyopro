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
    int N, K;
    cin >> N >> K;
    vector<ll> BC(N);
    vector<int> BG(N);
    int G = 0;
    rep(i,0,N){
        cin >> BC[i] >> BG[i];
        chmax(G, BG[i]);
        BG[i]--;
    }
    vector<vector<ll>> C(G,vector<ll>());
    rep(i,0,N){
        C[BG[i]].emplace_back(BC[i]);
    }
    rep(i,0,G){
        sort(all(C[i]),greater<ll>{});
        rep(j,0,C[i].size()){
            C[i][j] += 2*j;
        }
    }
    vector<vector<ll>> SC;
    rep(i,0,G){
        vector<ll> V(C[i].size()+1,0);
        rep(j,0,C[i].size()){
            V[j+1] = V[j] + C[i][j];
        }
        SC.emplace_back(V);
    }
    ll inf = (1LL<<60);
    vector dp(G+1,vector<ll>(N+1,-inf));
    dp[0][0] = 0;
    rep(i,0,G){
        int siz = SC[i].size();
        rep(j,0,N+1){
            if(dp[i][j] == -inf) continue;
            rep(k,j,N+1){
                if(k-j<siz) chmax(dp[i+1][k], dp[i][j]+SC[i][k-j]);
            }
        }
    }
    cout << dp[G][K] << endl;
}