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

// 

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;
    vector<string> S(N);
    rep(i,0,N) cin >> S[i];

    vector<vector<ll>> Black(N+1, vector<ll>(N+1, 0));
    rep(i,1,N+1){
        rep(j,1,N+1){
            Black[i][j] = Black[i][j-1] + (S[i-1][j-1] == 'B');
        }
    }
    rep(j,1,N+1){
        rep(i,1,N+1){
            Black[i][j] += Black[i-1][j];
        }
    }

    auto f = [&](int x, int y) -> ll {
        ll res = 0;
        x += 1; y += 1;
        ll k = x / N;
        ll l = y / N;
        res += k*l*Black[N][N] + k*Black[N][y%N] + l*Black[x%N][N] + Black[x%N][y%N];
        return res;
    };

    int a, b, c, d;
    for(;Q--;){
        cin >> a >> b >> c >> d;
        cout << f(c, d) - f(a-1, d) - f(c, b-1) + f(a-1, b-1) << endl;
    }
}