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
    ll N, K, X;
    cin >> N >> K >> X;
    vector<long double> A(N);
    rep(i,0,N) cin >> A[i];
    long double eps = 1e-10;
    long double ok = 1e9+1, ng = 0;
    auto f = [&](long double m) -> bool {
        vector<ll> res(N,0);
        rep(i,0,N){
            ll c = 1;
            long double a = A[i];
            while(a>m){
                a /= 2;
                res[i] += c;
                c <<= 1;
            }
        }
        ll cnt = 0;
        rep(i,0,N){
            cnt += res[i];
        }
        return cnt <= K;
    };
    while(ok-ng>eps){
        long double mid = (ok+ng)/2;
        (f(mid) ? ok : ng) = mid;
    }
    long double D = ok;
    pq<pair<long double,ll>> que;
    ll k = 0;
    rep(i,0,N){
        ll c = 1, cnt = 0;
        long double a = A[i];
        while(a>D){
            a /= 2;
            cnt += c;
            c <<= 1;
        }
        k += cnt;
        que.push(make_pair(a,c));
    }
    ll rem = K - k;
    long double ans;
    while(rem>0){
        auto p = que.top();
        que.pop();
        if(rem>=p.second){
            que.push(make_pair(p.first/2,p.second*2));
            rem -= p.second;
        }else{
            que.push(make_pair(p.first,p.second-rem));
            que.push(make_pair(p.first/2,rem*2));
            rem = 0;
        }
    }
    while(X>0){
        auto p = que.top();
        que.pop();
        if(X>p.second){
            X -= p.second;
        }else{
            ans = p.first;
            X = 0;
        }
    }
    printf("%.12Lf\n",ans);
}

int main()
{
    int T;
    cin >> T;
    while(T--) solve();
}