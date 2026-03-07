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

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, S;
    cin >> N >> S;
    vector<ll> A(N);
    rep(i,0,N) cin >> A[i];
    int N1 = N/2, N2 = N - N1;
    vector<ll> A1(N1), A2(N2);
    rep(i,0,N1) A1[i] = A[i];
    rep(i,N1,N) A2[i-N1] = A[i];
    vector<ll> S1((1<<N1),0), S2((1<<N2),0);
    rep(i,0,(1<<N1)){
        ll bit = i;
        rep(j,0,N1){
            if(bit>>j&1){
                S1[i] += A[j];
            }
        }
    }
    rep(i,0,(1<<N2)){
        ll bit = i;
        rep(j,0,N2){
            if(bit>>j&1){
                S2[i] += A[j+N1];
            }
        }
    }
    sort(all(S1));
    sort(all(S2));
    ll ans = 0;
    rep(i,0,(1<<N1)){
        ll k = lower_bound(all(S2),S-S1[i]+1) - lower_bound(all(S2),S-S1[i]);
        ans += k;
    }
    cout << ans << endl;
}