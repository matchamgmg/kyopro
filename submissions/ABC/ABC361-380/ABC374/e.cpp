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
    int N,X;
    cin >> N >> X;
    vll A(N),B(N),P(N),Q(N);
    rep(i,0,N){
        cin >> A[i] >> P[i] >> B[i] >> Q[i];
        if(A[i]*Q[i] < B[i]*P[i]){
            swap(A[i],B[i]);
            swap(P[i],Q[i]);
        }
    }

    auto f = [&](int m){
        ll cost = 0;
        rep(i,0,N){
            ll min_cost = 1e18;
            rep(j,0,A[i]){
                ll rest = max((ll)m-j*B[i],0LL);
                ll ai = (rest + A[i] - 1)/A[i];
                chmin(min_cost,ai*P[i]+j*Q[i]);
            }
            cost += min_cost;
            if(cost > X) return false;
        }
        return cost <= X;
    };

    int L = 0, R = 1e9+1;
    while(R-L>1){
        int M = (L+R)/2;
        (f(M) ? L : R) = M;
    }
    cout << L << endl;
}