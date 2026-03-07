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

// 円上のモンスターを直線状に写像
// 区間和
// 角度の大小関係

// long double (128bit) eps = 1.926e-19
// double (64bit) eps = 2.220e-16

ll gcd(ll a, ll b){
    if(a % b == 0){
        return b;
    }
    return gcd(b, a%b);
}

void regularlize(ll &a, ll &b){
    if(a == 0){
        b = 1;
        return;
    }
    if(b == 0){
        a = 1;
        return;
    }
    if(a < 0){
        a *= -1;
        b *= -1;
    }
    ll g = gcd(abs(a), abs(b));
    a /= g;
    b /= g;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;
    vector<pair<ll,ll>> P(N);
    rep(i,0,N) cin >> P[i].first >> P[i].second;

    vector<pair<ll,ll>> P2(N);
    rep(i,0,N){
        P2[i] = P[i];
        regularlize(P2[i].first, P2[i].second);
    }

    vector<long double> theta(N);

    rep(i,0,N){
        theta[i] = atan2l((long double)P2[i].second, (long double)P2[i].first);
        if(P[i].first < 0 || (P[i].first==0 && P[i].second < 0)){
            theta[i] += M_PI;
        }
    }

    const long double eps = 1e-18;
    vector<long double> E(N);
    rep(i,0,N){
        E[i] = theta[i];
    }

    sort(all(E), [&](long double i, long double j){ return i < j; });

    int A, B;
    rep(q,0,Q){
        cin >> A >> B;
        A--; B--;
        long double theta_A = theta[A];
        long double theta_B = theta[B];
        int b = lower_bound(E.begin(), E.begin()+N, theta_B) - E.begin();
        int a = upper_bound(E.begin(), E.begin()+N, theta_A) - E.begin();
        if(a - b > 0){
            cout << a - b << endl;
        }else{
            cout << N - (b - a) << endl;
        }
    }
}