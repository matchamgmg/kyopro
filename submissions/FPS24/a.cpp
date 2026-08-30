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

int main()
{
    int D,N;
    cin >> D >> N;
    vector<mint> kaijou(D+1);
    kaijou[0] = 1;
    rep(i,1,D+1){
        kaijou[i] = kaijou[i-1]*i;
    }
    auto calc_convolution = [&](mint a, mint b) -> mint {
        if(a.val()<b.val()){
            return mint(0);
        }else{
            return kaijou[a.val()] * kaijou[b.val()].inv() * kaijou[(a-b).val()].inv();
        }
    };
    //[x^(N-D)](1+x^2)^D(1+x^3)^D
    mint ans = 0;
    rep(i,0,N-D+1){
        //[x^i](1+x^2)^D
        mint bin2 = 0;
        if(i%2==0 && D>=(i/2)){
            bin2 = calc_convolution(mint(D),mint(i/2));
        }
        mint bin3 = 0;
        if((N-D-i)%3==0 && D>=(N-D-i)/3){
            bin3 = calc_convolution(mint(D),mint((N-D-i)/3));
        }
        ans += bin2*bin3;
    }
    cout << ans.val() << endl;
}