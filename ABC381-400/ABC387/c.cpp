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
    ll L,R;
    cin >> L >> R;
    // R以下のヘビ数 - L未満のヘビ数

    vll sigma(19,0);

    auto mypow = [](ll x,ll y) -> ll {
        ll res = 1;
        while(y>0){
            if(y&1) res *= x;
            x *= x;
            y >>= 1;
        }
        return res;
    };

    auto calc_sigma = [&](int n) -> ll {
        ll res = 0;
        rep(i,1,10){
            res += mypow(i,n-1);
        }
        return res;
    };

    rep(i,0,18){
        sigma[i+1] = sigma[i] + calc_sigma(i+1);
    }

    //v_cout(sigma);
    
    auto f = [&](const ll N) -> ll {
        ll res = 0;
        int d = 0;
        ll n = N;
        ll head = 0;
        while(n>0){
            d++;
            head = n;
            n /= 10;
        }
        // Nの桁より小さいヘビ数の個数
        res += sigma[d-1];
        // d桁の数であって先頭の数がNの先頭の数より小さい数以下のヘビ数の個数
        rep(i,1,head){
            res += mypow(i,d-1);
        }
        // d桁の数であって，Nの上からk-1桁まで一致し，k+1桁目の数がNのk+1桁目の数より小さい数
        // 以下のヘビ数の個数
        ll body = N - mypow(10,d-1)*head;
        d--;
        
        while(d>0){
            ll body_head = body / mypow(10,d-1);
            if(body_head<head){
                res += min(head,body_head)*mypow(head,d-1);
            }
            body -= mypow(10,d-1)*body_head;
            d--;
        }
        return res;
    };

    cout << (f(R) - f(L-1)) << endl;
}