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

    auto mypow = [](ll x, ll y) -> ll {
        ll res = 1;
        while(y>0){
            res *= x;
            y--;
        }
        return res;
    };
    
    auto f = [&](ll n) -> ll {
        vi digit;
        while(n){
            digit.push_back(n%10);
            n /= 10;
        }
        reverse(all(digit));
        int d = digit.size();
        ll res = 0;
        rep(i,1,d+1){
            if(i==d){
                res++;
                break;
            }
            res += mypow(digit[0],d-1-i) * min(digit[0], digit[i]);
            if(digit[i] >= digit[0]) break;
        }
        rep(i,0,d){
            int mx = (i ? 9 : digit[0]-1);
            rep(j,1,mx+1){
                res += mypow(j,d-1-i);
            }
        }
        return res;
    };

    cout << (f(R) - f(L-1)) << endl;
}