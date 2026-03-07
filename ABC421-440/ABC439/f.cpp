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

// 4 7 2 8 1 3 5 6
// 4 7 2, 4 7 1, 4 7 3, 4 7 5, 4 7 6
// 4 7 2 8

// i 番目の数を見た時
// 後ろ二つが x < y
// 後ろ二つが x > y

// i個目を見るとき
// 最後尾の値を管理する
// 最後尾がxである数列の個数

mint op(mint a, mint b){ return a + b; }

mint e(){ return mint(0); }


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vi P(N);
    rep(i,0,N) cin >> P[i];
    rep(i,0,N) P[i]--;

    // seg1 : x が登場するたびに +
    // seg2 : 長さ2以上で x で終わる数列の個数
    segtree<mint,op,e> seg1(N), seg2(N);

    vector<vector<mint>> dp(N+1, vector<mint>(2, 0));
    rep(i,0,N){
        // P[i] をとらない
        dp[i+1][0] = dp[i][0];
        dp[i+1][1] = dp[i][1];
        // P[i] をとる
        // 長さ2の数列
        mint l1 = seg1.prod(0,P[i]);
        dp[i+1][0] += l1;
        // 最後尾が x < P[i] の数列の個数
        mint l = seg2.prod(0,P[i]);
        mint r = seg2.prod(P[i]+1, N);
        dp[i+1][0] += l;
        // 最後尾が x > P[i] の数列の個数
        dp[i+1][1] += r;

        seg1.set(P[i], seg1.get(P[i]) + 1);
        seg2.set(P[i], seg2.get(P[i]) + l + r + l1);
    }
    cout << dp[N][1].val() << endl;
}