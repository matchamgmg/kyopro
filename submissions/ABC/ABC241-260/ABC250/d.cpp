#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
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

#define rep(i, s, n) for (int i = s; i < (n); i++)
#define rrep(i, s, n) for (int i = (n) - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n)rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}

int main()
{
    ll N;
    cin >> N;
    vll prime_number;
    int n = 1e6;
    /*エラトステネスの篩*/
    // O(N*log(log N))
    vector<bool> is_prime(n,true);
    is_prime[0] = false;
    is_prime[1] = false;
    rep(i,2,n){
        if(!is_prime[i]) continue;
        for(int j=i*2;j<n;j+=i){
            is_prime[j] = false;
        }
        prime_number.push_back(i);
    }
    ll ans = 0;
    int siz = prime_number.size();
    rep(i,1,siz){
        ll q_3 = prime_number[i]*prime_number[i]*prime_number[i];
        if(q_3>N) continue;
        int l = -1, r = i;
        while(r-l>1){
            int mid = (l+r)/2;
            auto P = [&](int m) -> bool {
                return prime_number[m] <= N/q_3;
            };
            (P(mid) ? l : r) = mid;
        }
        ans += l+1;
    }
    cout << ans << endl;
}