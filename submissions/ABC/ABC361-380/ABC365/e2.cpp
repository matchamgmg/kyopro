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
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}

int main()
{
    int N;
    cin >> N;
    vi A(N);
    int maxA = 0;
    rep(i,0,N){
        cin >> A[i];
        chmax(maxA,A[i]);
    }
    int d = 0;
    while(maxA>0){
        maxA >>= 1;
        d++;
    }
    vvi bitA(d,vi(N));
    rep(i,0,d){
        rep(j,0,N){
            bitA[i][j] = (A[j]>>i)&1;
        }
    }
    vvi ruiseki_bitA(d,vi(N+1,0));
    vi cnt_bit1(d,0), cnt_bit0(d,1);
    vi sum_bitA(d,0);
    rep(i,0,d){
        rep(j,0,N){
            ruiseki_bitA[i][j+1] = ruiseki_bitA[i][j] ^ bitA[i][j];
            (ruiseki_bitA[i][j+1]==1 ? cnt_bit1[i] : cnt_bit0[i])++;
            sum_bitA[i] += bitA[i][j];
        }
    }
    auto nC2 = [](int n) -> ll {
        return (ll)n*(n-1)/2;
    };
    ll ans = 0;
    rep(i,0,d){
        ll ansi = nC2(N+1) - nC2(cnt_bit1[i]) - nC2(cnt_bit0[i]);
        ansi -= sum_bitA[i];
        ansi <<= i;
        ans += ansi;
    }
    cout << ans << endl;
}