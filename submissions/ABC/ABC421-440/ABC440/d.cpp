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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, Q;
    cin >> N >> Q;
    vll A(N);
    rep(i,0,N) cin >> A[i];
    sort(all(A));
    vll Diff(N-1);
    rep(i,0,N-1) Diff[i] = A[i+1] - A[i] - 1;
    vll B(N+1,0);
    B[1] = A[0] - 1;
    rep(i,1,N) B[i+1] = B[i] + Diff[i-1];

    ll X, Y;
    while(Q--){
        cin >> X >> Y;
        auto itr_l = lower_bound(all(A),X);
        int l = itr_l - A.begin();
        // B[l+1] - (X-1-l) = 残り使える数
        ll k_l = B[l+1] - (X-1-l);
        if(k_l >= Y){
            cout << X + Y - 1 << endl; 
            continue;;
        }
        ll rem = Y - k_l;
        auto itr_r = lower_bound(all(B), rem+B[l+1]);
        auto itr_rr = prev(itr_r);
        int r = itr_r - B.begin();
        int rr = itr_rr - B.begin();
        rem -= B[rr] - B[l+1];
        ll ans = A[rr-1] + rem;
        cout << ans << endl;
    }
}