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

ll gcd(ll a, ll b){
    if(a%b==0) return b;
    return gcd(b,a%b);
}

int main()
{
    ll N,M,K;
    cin >> N >> M >> K;
    ll lcm = N*M / gcd(N,M);
    ll n = lcm / N;
    ll m = lcm / M;
    ll k;
    if(n+m==3){
        cout << min(N,M) + lcm*(K-1) << endl;
        return 0;
    }else{
        k = K % (n+m-2);
        int i = 0, j = 0;
        ll x = 0;
        while(i+j<k){
            if(N*(i+1) < M*(j+1)){
                i++;
                x = N*i;
            }else{
                j++;
                x = M*j;
            }
        }
        cout << lcm*(K / (n+m-2)) + x << endl;
    }

}