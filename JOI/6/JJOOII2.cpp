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
    int N, K;
    cin >> N >> K;
    string S;
    cin >> S;
    vi J(N+1,0), O(N+1,0), I(N+1,0);
    rep(i,0,N){
        J[i+1] = J[i] + (S[i]=='J');
        O[i+1] = O[i] + (S[i]=='O');
        I[i+1] = I[i] + (S[i]=='I');
    }
    vi IJ, IO, II;
    rep(i,0,N){
        if(S[i]=='J') IJ.emplace_back(i);
        else if(S[i]=='O') IO.emplace_back(i);
        else II.emplace_back(i);
    }

    const int INF = 1<<29;
    int ans = INF;
    rep(i,0,IJ.size()){
        int io, ii;
        auto f = [&](int m, int x) -> bool {
            return J[IO[m]]-J[IJ[x]]>=K;
        };
        int ng = -1, ok = IO.size();
        while(ok-ng>1){
            int mid = (ok+ng)/2;
            (f(mid,i) ? ok : ng) = mid;
        }
        if(ok != IO.size()){
            io = ok;
        }else continue;
        auto ff = [&](int m, int x) -> bool {
            return O[II[m]]-O[IO[x]]>=K;
        };
        ng = -1, ok = II.size();
        while(ok-ng>1){
            int mid = (ok+ng)/2;
            (ff(mid,io) ? ok : ng) = mid;
        }
        if(ok != II.size()){
            ii = ok;
        }else continue;
        auto fff = [&](int m, int x) -> bool {
            return I[II[m]+1]-I[II[x]]>=K;
        };
        ng = -1, ok = II.size();
        while(ok-ng>1){
            int mid = (ok+ng)/2;
            (fff(mid,ii) ? ok : ng) = mid;
        }
        if(ok != II.size()){
            ii = ok;
        }else continue;
        chmin(ans, II[ii]+1-IJ[i]-3*K);
    }
    cout << (ans == INF ? -1 : ans) << endl;
}