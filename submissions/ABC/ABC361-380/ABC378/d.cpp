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

vector<int> dx = {0,1,0,-1}, dy = {1,0,-1,0};

int dfs_sub(vector<string> &S, vector<vector<bool>> &used, int x, int y, const int K, int n, const int H, const int W){
    if(n==K){
        return 1;
    }else{
        int ret = 0;
        rep(i,0,4){
            int xx = x + dx[i];
            int yy = y + dy[i];
            if((0<=xx&&xx<H&&0<=yy&&yy<W) && (S[xx][yy]=='.') && (used[xx][yy] == false)){
                used[xx][yy] = true;
                ret += dfs_sub(S,used,xx,yy,K,n+1,H,W);
                used[xx][yy] = false;
            }
        }
        return ret;
    }
}

int main()
{
    int H,W,K;
    cin >> H >> W >> K;
    vector<string> S(H);
    vector<vector<bool>> used(H,vector<bool>(W,false));
    rep(i,0,H){
        cin >> S[i];
    }
    int ans = 0;
    rep(i,0,H){
        rep(j,0,W){
            if(S[i][j]=='.'){
                used[i][j] = true;
                ans += dfs_sub(S,used,i,j,K,0,H,W);
                used[i][j] = false;
            }
        }
    }
    cout << ans << endl;
}