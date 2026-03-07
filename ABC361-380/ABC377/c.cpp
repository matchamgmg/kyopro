#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
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
template<class T> using pq = priority_queue<T, vector<T>>;//大きい順
template<class T> using pq_g = priority_queue<T, vector<T>, greater<T>>;//小さい順

#define rep(i, s, n) for (int i = s; i < (n); i++)
#define rrep(i, s, n) for (int i = (n) - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n)rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}

int main()
{
    ll N;
    cin >> N;
    ll ans = N*N;
    int M;
    cin >> M;
    set<pair<int,int>> A;
    vi dx = {2,1,-1,-2,-2,-1,1,2}, dy = {1,2,2,1,-1,-2,-2,-1};
    rep(i,0,M){
        int a,b;
        cin >> a >> b;
        a--;
        b--;
        if(!A.count({a,b})){//koma
            ans--;
            A.insert({a,b});
        }
        //idou
        rep(i,0,8){
            int x = a + dx[i];
            int y = b + dy[i];
            if(0<=x&&x<N&&0<=y&&y<N){
                if(!A.count({x,y})){
                    ans--;
                    A.insert({x,y});
                }
            }
        }
    }
    cout << ans << endl;
}