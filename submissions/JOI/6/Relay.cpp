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

// 走る3人が決まったとき，三人の走る順番も決まる
// バトンパスがうまい人が真ん中
// Bの昇順に見る
// A1 + A2 + A3 + B2 + B3 -- 

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vi A(N), B(N);
    rep(i,0,N) cin >> A[i] >> B[i];

    vi idx(N);
    iota(all(idx),0);
    sort(all(idx), [&](int i, int j){ return B[i] < B[j]; });

    // 真ん中の走者を固定したとき，残り二人はA+Bの小さいもの
    const int INF = 1<<28;
    int min1, min2;
    min1 = INF;
    min2 = INF;
    int ans = INF*5;
    rrep(i,0,N){
        chmin(ans, A[idx[i]]+min1+min2);

        int a = A[idx[i]] + B[idx[i]];
        if(a < min1){
            min2 = min1;
            min1 = a;
        }else if(a < min2){
            min2 = a;
        }
    }
    cout << ans <<endl;
}