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
    double eps = 1e-10;
    int N, W, H;
    cin >> N >> W >> H;
    vector<int> X(N), Y(N);
    rep(i,0,N){
        cin >> X[i] >> Y[i];
    }
    auto is_in = [&](int a, int b) -> bool {
        int dd = (X[a]-X[b])*(X[a]-X[b]) + (Y[a]-Y[b])*(Y[a]-Y[b]);
        double d = sqrt(dd);
        return (-eps<X[a]-d && X[a]+d<W+eps && -eps<Y[a]-d && Y[a]+d<H+eps);
        // 長方形の輪郭への最短距離
        // min(X,W-X,Y,H-Y)
    };
    auto solve = [&](vector<int> &V) -> bool {
        if(!is_in(V[0],V[1])) return false;
        if(!is_in(V[2],V[3])) return false;
        int dd1 = (X[V[0]]-X[V[1]])*(X[V[0]]-X[V[1]]) + (Y[V[0]]-Y[V[1]])*(Y[V[0]]-Y[V[1]]);
        int dd2 = (X[V[2]]-X[V[3]])*(X[V[2]]-X[V[3]]) + (Y[V[2]]-Y[V[3]])*(Y[V[2]]-Y[V[3]]);
        int dd3 = (X[V[0]]-X[V[2]])*(X[V[0]]-X[V[2]]) + (Y[V[0]]-Y[V[2]])*(Y[V[0]]-Y[V[2]]);
        double d1 = sqrt(dd1), d2 = sqrt(dd2), d3 = sqrt(dd3);
        return (d1 > d2+d3+eps);
    };
    int ans = 0;
    rep(i,0,N-3){
        rep(j,i+1,N-2){
            rep(k,j+1,N-1){
                rep(l,k+1,N){
                    vector<int> ABCD(4);
                    ABCD[0] = i, ABCD[1] = j, ABCD[2] = k, ABCD[3] = l;
                    do{
                        ans += solve(ABCD);
                    }while(next_permutation(all(ABCD)));
                }
            }
        }
    }
    cout << ans << endl;
}