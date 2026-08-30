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

// x^n + y^n = k を満たすx, yの組の数をA[k] とする
// このとき，k > 1 に対して A[k] = A[1] が成り立つ
// x^n + y^n = 1 ==> (xt)^n + (yt)^n = t^n
// x^n + y^n = z^n ==> (xz^(-1))^n + (yz^(-1))^n = 1
// ただし 1 < t < p 

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int P, N;
    cin >> P >> N;

    vector<int> XN(P);

    auto pow = [&](int x, int y) -> int {
        int res = 1;
        while(y>0){
            if(y & 1) res *= x;
            res %= P;
            y >>= 1;
            x *= x;
            x %= P;
        }
        return res;
    };

    rep(i,0,P){
        XN[i] = pow(i, N);
    }
    vector<int> cnt(P,0);
    rep(i,0,P){
        cnt[XN[i]]++;
    }

    ll sum0 = 0, sum1 = 0;
    rep(i,0,P){
        sum0 += cnt[i] * cnt[(P-i)%P];
        sum1 += cnt[i] * cnt[(P+1-i)%P];
    }
    ll ans = sum0;
    rep(i,1,P){
        ans += cnt[i] * sum1;
    }
    cout << ans << endl;
}