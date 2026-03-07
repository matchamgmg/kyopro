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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    vector G(N, set<int>());
    int a, b;
    rep(i,0,M){
        cin >> a >> b;
        a--; b--;
        G[a].emplace(b);
        G[b].emplace(a);
    }

    vector<int> V(N);
    iota(all(V),0);
    const int INF = 1<<29;
    int ans = INF;

    auto solve = [&](const vector<int> &v, int siz1, int siz2) -> int {
        int have = 0;
        rep(i,0,siz1){
            int from = v[i];
            int to = v[(i+1)%siz1];
            have += (G[from].count(to));
        }
        rep(i,siz1,N){
            int from = v[i];
            int to = v[siz1 + (i+1)%siz2];
            have += (G[from].count(to));
        }
        return M + N - have*2;
    };

    do{
        chmin(ans, solve(V,N,0));
    }while(next_permutation(all(V)));

    if(N == 8){
        // 3:5, 4:4
        sort(all(V));
        do{
            chmin(ans,solve(V,3,5));
            chmin(ans,solve(V,4,4));
            chmin(ans,solve(V,5,3));
        }while(next_permutation(all(V)));
    }else if(N == 7){
        // 3:4
        sort(all(V));
        do{
            chmin(ans,solve(V,3,4));
            chmin(ans,solve(V,4,3));
        }while(next_permutation(all(V)));
    }else if(N == 6){
        sort(all(V));
        do{
            chmin(ans,solve(V,3,3));
        }while(next_permutation(all(V)));
    }

    cout << ans << endl;
}