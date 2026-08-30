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
    ll K;
    cin >> N >> M >> K;
    vll A(N);
    ll rem = K;
    rep(i,0,N){
        cin >> A[i];
        rem -= A[i];
    }
    if(N==M){
        rep(i,0,N) cout << 0 << " \n"[i==N-1];
        return 0;
    }
    vll B = A;
    sort(all(B),greater<ll>{});
    vll C(N+1,0);
    rep(i,0,N) C[i+1] = C[i] +B[i];

    vll ans(N);

    auto f = [&](int i, ll gain){
        ll border = A[i] + gain;
        int already_than_border = [&](){
            int L = -1, R = M;
            while(R-L>1){
                int mid = (R+L)/2;
                (B[mid]<=border ? R : L) = mid;
            }
            return R;
        }();

        //A[i] + gain <= border
        if(M-already_than_border<=0) return false;
        ll sum = C[M] + (A[i]>=B[M-1] ? -A[i]+B[M] : 0) - C[already_than_border];
        return sum + (rem-gain) < (border+1)*(M-already_than_border);
    };

    rep(i,0,N){
        ll gainL = -1, gainR = rem+1;
        while(gainR-gainL>1){
            ll gainM = (gainL+gainR)/2;
            (f(i,gainM) ? gainR : gainL) = gainM;
        }
        ans[i] = (gainR==rem+1 ? -1 : gainR);
    }
    v_cout(ans);
}