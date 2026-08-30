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
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]
template<class T>
bool chmax(T &a, T b){ if(a < b){ a = b; return true; } else return false; }
template<class T>
bool chmin(T &a, T b){ if(a > b){ a = b; return true; } else return false; }

// N = 110100, D = 6, K = 3
// d = 1<<5 = 100000
// 110100 ~ 110100
// 110000 ~ 110100 : D = 2, K = 1
// 100000 ~ 110000 : D = 4, K = 2
//      0 ~ 100000 : D = 5, K = 3

mint DFS(ll N, ll M, ll K, ll D, vector<vector<mint>> &combination){
    if(D == 0) return mint(0);
    if(K < 0) return mint(0);
    if(K > D) return mint(0);
    ll NM = M + (1LL<<(D-1));
    mint res(0);
    if(N > NM){
        if(D>=2 && K>=1) res += combination[D-2][K-1]*mint((1LL<<(D-1))-1) + M*combination[D-1][K];
        res += DFS(N,NM,K-1,D-1,combination);
        if(K == 1) res += mint(NM);
    }else if(N == NM){
        if(D>=2 && K>=1) res += combination[D-2][K-1]*mint((1LL<<(D-1))-1) + M*combination[D-1][K];
        if(K == 1) res += mint(N);
    }else{
        res += DFS(N,M,K,D-1,combination);
    }
    return res;
}

void solve(vector<vector<mint>> &combination){
    ll N, K;
    cin >> N >> K;
    ll d = 0, n = N;
    while(n){
        d++;
        n >>= 1;
    }
    cout << DFS(N,0,K,d,combination).val() << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<vector<mint>> combination(61, vector<mint>(61,0));
    rep(i,0,61){
        combination[i][0] = 1;
        rep(j,1,i+1){
            combination[i][j] = combination[i][j-1]*mint(i-j+1)*mint(j).inv();
        }
    }

    int T;
    cin >> T;
    for(;T--;) solve(combination);
}