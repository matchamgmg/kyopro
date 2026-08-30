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

// 1 3 4
// max(1,3)/min(1,3) + max(1,4)/min(1,4) + max(3,4)/min(3,4)
// min(Ai,Aj) = Ai, max(Ai,Aj) = Aj
// sum(floar(max(Ai,Aj)/min(Ai,Aj))) = sum(floar(Aj/Ai))
// iを固定したとき floar(Aj/Ai)の値を動かす
// f(Ai, k) := floar(Aj/Ai) = k を満たすjの個数
// sum_{i=0..N-1}sum_{k}(k * f(Ai,k))

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    ll A_max = 0;
    vector<ll> A(N);
    rep(i,0,N) cin >> A[i];
    rep(i,0,N) chmax(A_max, A[i]);
    sort(all(A));

    vector<ll> C(A_max+1,0);
    rep(i,0,N){
        C[A[i]]++;
    }
    rep(i,0,A_max){
        C[i+1] += C[i];
    }

    ll ans = 0;

    ll cnt = 1;
    rep(i,0,N-1){
        if(i+1<N-1 && A[i] == A[i+1]){
            cnt++;
            continue;
        }
        for(ll n=A[i]; n<=A_max; n+=A[i]){
            // Aj/Ai = nとなるjの個数
            ll j = C[min(A_max,n+A[i]-1)] - C[max(A[i]-1,n-1)];
            j *= cnt;
            if(n == A[i]) j -= cnt*(cnt+1)/2;
            ans += j*(n/A[i]);
            // cout << "n = " << n << " : j = " << j << endl;
        }
        cnt = 1;
    }
    cout << ans << endl;
}