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

    // 1 <= b < a <= N かつ a は b の倍数でないような(a,b)の個数
    // for b = 1::N : (N - b) - (N//b - 1)
    // N*(N+1)/2 - sum N//b

    ll N;
    cin >> N;
    mint n(N);

    mint ans = n*(n+1)*mint(2).inv();
    auto root = [](ll x) -> ll {
        ll res = sqrt(x);
        if((res+1)*(res+1) <= x) res++;
        if(res*res > x) res--;
        return res;
    };

    // b > root(N) のとき N//b < root(N)
    // for i = 1::root(N) : 
    // 1 : N//1 ~ N//2 + 1
    // 2 : N//2 ~ N//3 + 1

    ll rootN = root(N);
    rep(i,1,rootN+1){
        ans -= mint(i)*mint(N/i - (N/(i+1)+1) + 1);
    }

    // b <= root(N) のとき
    rep(i,1,rootN+1){
        if(N/i <= rootN) continue;
        ans -= N/i;
    }

    cout << ans.val() << endl;
}