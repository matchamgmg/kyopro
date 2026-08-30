#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
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
template<class T> using pq = priority_queue<T, vector<T>>;//大きい順
template<class T> using pq_g = priority_queue<T, vector<T>, greater<T>>;//小さい順

#define rep(i, s, n) for (int i = s; i < (n); i++)
#define rrep(i, s, n) for (int i = (n) - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n)rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}

int main()
{
    int N;
    ll M;
    cin >> N >> M;
    vll A(N);
    ll sum_A = 0;
    rep(i,0,N){
        cin >> A[i];
        sum_A += A[i];
    }
    if(sum_A<=M){
        cout << "infinite" << endl;
        return 0;
    }
    ll L = 0, R = M;
    while(R-L>1){
        auto P = [&](ll m) -> bool {
            ll sum_expenses = 0;
            rep(i,0,N){
                sum_expenses += min(m,A[i]);
            }
            return sum_expenses <= M;
        };
        ll mid = (L+R)/2;
        (P(mid) ? L : R) = mid;
    }
    ll ans = L;
    cout << ans << endl;
}