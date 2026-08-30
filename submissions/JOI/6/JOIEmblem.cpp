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
    int M,N;
    cin >> M >> N;
    vector<string> S(M);
    rep(i,0,M) cin >> S[i];
    vector<string> T(2);
    rep(i,0,2) cin >> T[i];
    vector<char> JOI = {'J','O','I'};
    auto f = [&](int i, int j){
        if(i<0 || M<=i+1 || j<0 || N<=j+1) return 0;
        rep(ii,0,2){
            rep(jj,0,2){
                if(S[ii+i][jj+j] != T[ii][jj]) return 0;
            }
        }
        return 1;
    };
    auto score = [&](int i, int j){
        int res = 0;
        res = f(i-1,j-1) + f(i-1,j) + f(i,j-1) + f(i,j);
        return res;
    };
    int ans = 0;
    rep(i,0,M){
        rep(j,0,N){
            ans += f(i,j);
        }
    }
    int before, after;
    char pc;
    int add = 0;
    rep(i,0,M){
        rep(j,0,N){
            before = score(i,j);
            pc = S[i][j];
            rep(k,0,3){
                S[i][j] = JOI[k];
                after = score(i,j);
                chmax(add, after-before);
            }
            S[i][j] = pc;
        }
    }
    cout << ans + add << endl;
}