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

ll my_pow(ll a, ll b){
    ll res = 1;
    rep(i,0,b){
        res *= a;
    }
    return res;
}

int main()
{
    ll N;
    cin >> N;
    if(N==1){
        cout << 0 << endl;
        return 0;
    }
    N--;
    vll keta_num(36);
    rep(i,0,36){
        keta_num[i] = 9*my_pow(10,i/2);
    }
    vll ruiseki(37,0);
    rep(i,0,36){
        ruiseki[i+1] = ruiseki[i] + keta_num[i];
    }
    int idx = 0;
    while(idx<=36 && N > ruiseki[idx]) idx++;
    int ans_len = (idx+2-1)/2;
    vi ans(ans_len,0);
    ans[0] = 1;
    ll n = N - ruiseki[idx-1] - 1;
    rep(i,0,ans_len){
        ans[ans_len-1-i] += n%10;
        n /= 10;
    }
    rep(i,0,ans_len){
        cout << ans[i];
    }
    rrep(i,0,ans_len){
        if(i==ans_len-1 && idx%2==1) continue;
        cout << ans[i];
    }
    cout << endl;
}