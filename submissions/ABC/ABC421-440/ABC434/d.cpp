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
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]

int main()
{
    int siz = 2000;
    vector<vector<ll>> F(siz+2,vector<ll>(siz+2,0));
    vector<vector<bool>> V(siz+2,vector<bool>(siz+2,true));
    int N;
    cin >> N;
    vi U(N),D(N),L(N),R(N);
    rep(i,0,N){
        cin >> U[i] >> D[i] >> L[i] >> R[i];
        F[U[i]][L[i]] += 1;
        F[U[i]][R[i]+1] += -1;
        F[D[i]+1][L[i]] += -1;
        F[D[i]+1][R[i]+1] += 1;
    }
    rep(i,1,siz+1){
        rep(j,1,siz+1){
            F[i][j+1] = F[i][j] + F[i][j+1];
        }
    }
    rep(i,1,siz+1){
        rep(j,1,siz+1){
            F[i+1][j] = F[i][j] + F[i+1][j];
        }
    }
    ll zero = 0;

    rep(i,1,siz+1){
        rep(j,1,siz+1){
            if(F[i][j]==0){
                zero++;
            }else if(F[i][j]==1){
                F[i][j] = 0;
            }else{
                F[i][j] = 0;
                V[i][j] = false;
            }
        }
    }
    rep(i,0,N){
        F[U[i]][L[i]] += i+1;
        F[U[i]][R[i]+1] += -(i+1);
        F[D[i]+1][L[i]] += -(i+1);
        F[D[i]+1][R[i]+1] += i+1;
    }
    rep(i,1,siz+1){
        rep(j,1,siz+1){
            F[i][j+1] = F[i][j] + F[i][j+1];
        }
    }
    rep(i,1,siz+1){
        rep(j,1,siz+1){
            F[i+1][j] = F[i][j] + F[i+1][j];
        }
    }
    vll ans(N+1);
    rep(i,1,siz+1){
        rep(j,1,siz+1){
            if(F[i][j]<=0){
                continue;
            }else{
                if(V[i][j]) ans[F[i][j]]++;
            }
        }
    }
    rep(i,1,N+1){
        cout << ans[i] + zero << endl;
    }
}