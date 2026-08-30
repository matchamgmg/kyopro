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

int main()
{
    int N,M;
    cin >> N >> M;
    vll B(N),W(M);
    rep(i,0,N){
        cin >> B[i];
    }
    rep(i,0,M){
        cin >> W[i];
    }
    sort(all(B),greater<ll>{});
    sort(all(W),greater<ll>{});
    vll rui_B(N+1,0), rui_W(M+1,0);
    rep(i,0,N){
        rui_B[i+1] = rui_B[i] + B[i];
    }
    rep(i,0,M){
        rui_W[i+1] = rui_W[i] + W[i];
    }
    ll ans = -2*1000000000000000;
    int L = -1,R = N;
    auto f = [&](int m){
        return B[m] >= 0;
    };
    while(R-L>1){
        int mid = (R+L)/2;
        (f(mid) ? L : R) = mid;
    }
    int B_idx = L+1;
    rep(i,0,M+1){
        if(i <= B_idx){
            chmax(ans,rui_W[i] + rui_B[B_idx]);
        }else{
            if(i<N+1){
                chmax(ans,rui_W[i] + rui_B[i]);
            }else{
                continue;
            }
        }
    }
    cout << ans << endl;
}