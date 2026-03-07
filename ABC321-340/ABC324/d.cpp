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

ll mypow(ll a,ll b){
    ll res = 1;
    while(b>0){
        if(b&1) res *= a;
        a *= a;
        b>>=1;
    }
    return res;
}

int main()
{
    int N;
    string S;
    cin >> N >> S;
    vi backet(10,0);
    rep(i,0,N){
        backet[S[i]-'0']++;
    }
    sort(all(S));
    ll s = 0;
    rep(i,0,N){
        s += (S[i]-'0')*mypow(10,i);
    }
    int ans = 0;
    for(ll i = 0;i*i<=s;i++){
        bool ok = true;
        ll x = i*i;
        vi backet_x(10,0);
        backet_x[0] = N;
        int n = 0;
        while(n<N){
            if(x%10!=0){
                backet_x[x%10]++;
                backet_x[0]--;
            }
            x /= 10;
            n++;
        }
        rep(j,0,10){
            if(backet[j]!=backet_x[j]){
                ok = false;
            }
        }
        if(ok){
            ans++;
        }
    }
    cout << ans << endl;
}