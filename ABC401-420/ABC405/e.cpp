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


mint kaijou(mint n){
    if(n.val() <= 1) return n;
    return n*kaijou(n-1);
}

mint calc(mint a, mint b){
    mint res = kaijou(a+b)*kaijou(a).inv()*kaijou(b).inv();
    return res;
}

int main()
{
    ll A,B,C,D;
    cin >> A >> B >> C >> D;
    // ブドウの左端
    vector<mint> K(C+1,0); // 左端のブドウの左側にあるバナナの個数
    K[C] = 1;
    rrep(i,1,C+1){
        K[i-1] = (K[i]*mint(D+C-i))*mint(C-i+1).inv();
    }
    vector<mint> L(C+1,0);
    L[0] = calc(mint(A),mint(B));
    rep(i,0,C+1){
        L[i+1] = L[i]*mint(A+B+i+1)*mint(A+i+1).inv();
    }
    vector<mint> N(C+1,0);
    rep(i,0,C+1){
        N[i] = L[i]*K[i];
    }
    mint ans = 0;
    rep(i,0,C+1){
        ans += N[i];
    }
    cout << ans.val() << endl;
}