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
    int N,M;
    cin >> N >> M;
    vll C(N);
    rep(i,0,N){
        cin >> C[i];
    }
    vi K(M);
    vvi A(N);
    rep(i,0,M){
        cin >> K[i];
        rep(j,0,K[i]){
            int a;
            cin >> a;
            a--;
            A[a].push_back(i);
        }
    }
    int D = 2*N;
    const ll inf = 1LL<<61;
    ll ans = inf;
    vector<int> bits(D,0);
    for(int i = 0;i<(1<<D);i++){
        int b = i;
        vector<int> cnt(M,0);
        ll cost = 0;
        rep(j,0,D){
            bits[j] = (b&1);
            b >>= 1;
            if(bits[j]){
                cost += C[j%N];
                for(int m : A[j%N]){
                    cnt[m]++;
                }
            }
        }
        bool ok = true;
        rep(j,0,M){
            ok &= cnt[j]>=2;
        }
        if(ok){
            chmin(ans,cost);
        }
    }
    cout << ans << endl;
}