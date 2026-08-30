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
    int T;
    cin >> T;
    vmint kaijou(61,1);
    rep(i,0,61){
        kaijou[i+1] = kaijou[i]*(i+1);
    }
    rep(i,0,T){
        ll N;
        int K;
        cin >> N >> K;
        vi D;
        int d = 0;
        ll n = N;
        while(n>0){
            D.push_back(n&1);
            n >>= 1;
            d++;
        }
        mint ans = 0;
        mint Ans = 0;
        auto comb = [&](int a,int b) -> mint {
            if(a<=0) return mint(1);
            if(b<=0) return mint(1);
            mint res = kaijou[a]*mint(a-b).inv()*mint(b).inv();
            return res;
        };
        auto dfs = [&](auto f,int cnt, int dep,ll state) -> void {
            if(dep<0){
                ans++;
                Ans += N;
                return;
            }
            if(cnt>K) return;
            if(D[dep]==1){
                ans += comb(dep,K-cnt);
                Ans += mint(state)*comb(dep,K-cnt) + 
                f(f,cnt+1,dep-1);
            }else{
                f(f,cnt,dep-1);
            }
        };
        dfs(dfs,0,d-1);
        cout << ans.val() << endl;
    }
}