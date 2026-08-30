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

void solve(){
    int N, C;
    cin >> N >> C;
    C--;
    vector<string> S(N);
    rep(i,0,N) cin >> S[i];
    const int INF = 1<<29;
    
    vector<bool> dp(N,false);
    dp[C] = true;
    vector<bool> beakable(N,false);
    rep(i,0,N) if(S[N-1][i] == '.') beakable[i] = true;
    rrep(i,0,N-1){
        vector<bool> next_dp(N, false);
        rep(j,0,N){
            rep(k,-1,2){
                if(j+k<0 || j+k>=N) continue;
                if(dp[j] && (S[i][j+k]=='.'||(S[i][j+k]=='#'&&beakable[j+k]))){
                    next_dp[j+k] = true;
                }
            }
        }
        rep(j,0,N){
            if(!next_dp[j] && S[i][j]=='#') beakable[j] = false;
        }
        swap(dp,next_dp);
    }
    rep(i,0,N){
        cout << (dp[i] ? 1 : 0);
    }
    cout << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    for(;T--;) solve();
}