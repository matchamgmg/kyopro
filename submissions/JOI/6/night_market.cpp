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
    int N,T,S;
    cin >> N >> T >> S;
    vector<int> A(N), B(N);
    rep(i,0,N){
        cin >> A[i] >> B[i];
    }

    int inf = 1e9;
    vector dp1(N+1, vector<int>(T+1,-inf)), dp2(N+1, vector<int>(T+1,-inf));
    dp1[0][0] = 0, dp2[0][0] = 0;
    rep(i,0,N){
        rep(j,0,T+1){
            {
                if(j<T) chmax(dp1[i][j+1], dp1[i][j]);
                chmax(dp1[i+1][j], dp1[i][j]);
                if(j-B[i]>=0) chmax(dp1[i+1][j], dp1[i][j-B[i]]+A[i]);
            }
            {
                if(j<T) chmax(dp2[i][j+1], dp2[i][j]);
                chmax(dp2[i+1][j], dp2[i][j]);
                if(j-B[N-i-1]>=0) chmax(dp2[i+1][j], dp2[i][j-B[N-i-1]]+A[N-i-1]);
            }
        }
    }

    // vv_cout(dp1);
    // cout << endl;
    // vv_cout(dp2);
    // cout << endl;
    int ans = 0;
    rep(i,0,N){
        chmax(ans, dp1[i][S] + dp2[N-i][T-S]);
    }
    cout << ans << endl;
}