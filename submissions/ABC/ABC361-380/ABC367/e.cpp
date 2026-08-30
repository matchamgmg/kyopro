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
    int N;
    ll K;
    cin >> N >> K;
    vi X(N),A(N);
    rep(i,0,N){
        cin >> X[i];
        X[i]--;
    }
    rep(i,0,N){
        cin >> A[i];
    }
    // doubling
    vvi doubling(62,vi(N));
    // dp[i] : 2^i回目の操作
    rep(i,0,62){
        if(i==0){
            rep(j,0,N){
                doubling[i][j] = X[j];
            }
        }else{
            rep(j,0,N){
                doubling[i][j] = doubling[i-1][doubling[i-1][j]];
            }
        }
    }
    vi idx(N);
    iota(all(idx),0);
    ll k = K;
    int n = 0;

    auto query = [&](vi &idx,vi &dpi){
        vi new_idx(N);
        rep(i,0,N){
            new_idx[i] = idx[dpi[i]];
        }
        rep(i,0,N){
            idx[i] = new_idx[i];
        }
    };

    while(k>0){
        if(k&1){
            query(idx,doubling[n]);
        }
        n++;
        k>>=1;
    }
    rep(i,0,N){
        printf("%d ",A[idx[i]]);
    }
    cout << endl;
}