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
    ll N;
    cin >> N;
    vll prime_number;
    vector<bool> is_prime(2*1000000,true);
    is_prime[0] = false;
    is_prime[1] = false;
    for(int i=2;i<(int)is_prime.size();i++){
        if(is_prime[i]==true){
            prime_number.push_back(i);
            for(int j=i+i;j<(int)is_prime.size();j+=i){
                is_prime[j] = false;
            }
        }
    }
    ll ans = 0;
    rep(i,0,prime_number.size()){
        ll a = prime_number[i];
        if(a*a>=N) break;
        rep(j,i+1,prime_number.size()){
            ll b = prime_number[j];
            if(b*b>N/(a*a)) break;
            ans++;
        }
    }
    rep(i,0,prime_number.size()){
        ll x = prime_number[i];
        if(x>200) break;
        if(x*x*x*x*x*x*x*x>N) break;
        ans++;
    }
    cout << ans << endl;
}