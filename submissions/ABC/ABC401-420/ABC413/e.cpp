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

void solve(){
    int N;
    cin >> N;
    vi P(1<<N);
    rep(i,0,(1<<N)) cin >> P[i]; 

    auto DFS = [&](auto f, int b, vector<int> &V) -> void {
        if(b == 0) return;
        int width = 1<<b;

        auto g = [&](int l, int r, vector<int> &t) -> int {
            int res = 1<<N;
            rep(i,l,r){
                chmin(res, t[i]);
            }
            return res;
        };

        for(int a=0; a*width<(1<<N); ++a){
            rep(i,0,2){
                int ml = g(a*width, a*width+(width>>1), V);
                int mr = g(a*width+(width>>1), (a+1)*width, V);
                if(mr < ml) reverse(V.begin()+a*width, V.begin()+(a+1)*width);
            }
        }

        f(f, b-1, V);
        return;
    };

    DFS(DFS,N,P);
    v_cout(P);
}

int main()
{
    int T;
    cin >> T;
    while(T--) solve();
}