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

// 45度回転すると
// |x1-x2| + |y1-y2| -> max(|X1-X2|,|Y1-Y2|)

const ll INF = 1LL<<60;

ll op_max(ll a, ll b){ return max(a, b); }

ll op_min(ll a, ll b){ return min(a, b); }

ll e_max(){ return -INF; }

ll e_min(){ return INF; }

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;

    segtree<ll,op_max,e_max> segX_max(N), segY_max(N);
    segtree<ll,op_min,e_min> segX_min(N), segY_min(N);

    rep(i,0,N){
        ll x, y;
        cin >> x >> y;
        segX_max.set(i, x-y);
        segY_max.set(i, x+y);
        segX_min.set(i, x-y);
        segY_min.set(i, x+y);
    }

    ll t, k, x, y, L, R;
    for(;Q--;){
        cin >> t;
        if(t == 1){
            cin >> k >> x >> y;
            k--;
            segX_max.set(k, x-y);
            segY_max.set(k, x+y);
            segX_min.set(k, x-y);
            segY_min.set(k, x+y);
        }else{
            cin >> L >> R >> x >> y;
            L--;
            ll X = x - y;
            ll Y = x + y;
            // X軸Y軸別々に見て
            // Xから最も遠いやつ
            ll max_X = segX_max.prod(L, R);
            ll min_X = segX_min.prod(L, R);
            max_X = max(abs(max_X - X), abs(min_X - X));
            // Yから最も遠いやつ
            ll max_Y = segY_max.prod(L, R);
            ll min_Y = segY_min.prod(L, R);
            max_Y = max(abs(max_Y - Y), abs(min_Y - Y));
            cout << max(max_X, max_Y) << endl;
        }
    }
}