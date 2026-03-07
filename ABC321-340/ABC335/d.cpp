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
    cin >> N;
    vvi S(N,vi(N,0));
    int xt = (N-1)/2, yt = (N-1)/2;
    S[xt][yt] = 1;
    vi dx = {0,1,0,-1}, dy = {1,0,-1,0};
    int x = xt, y = yt;
    int i = 0;
    int cnt = 2;
    while(grid_check(x,y,N,N)){
        int xx = x + dx[i];
        int yy = y + dy[i];
        int xxx = x + dx[(i+1)%4];
        int yyy = y + dy[(i+1)%4];
        if(grid_check(xxx,yyy,N,N) && S[xxx][yyy]==0){
            S[xxx][yyy] = cnt;
            cnt++;
            i = (i+1)%4;
            x = xxx;
            y = yyy;
        }else if(grid_check(xx,yy,N,N) && S[xx][yy]==0){
            S[xx][yy] = cnt;
            cnt++;
            x = xx;
            y = yy;
        }else{
            x = xx;
            y = yy;
        }
    }
    rep(i,0,N){
        rep(j,0,N){
            if(S[i][j]==1){
                printf("T ");
            }else{
                printf("%d ",S[i][j]-1);
            }
        }
        printf("\n");
    }
}