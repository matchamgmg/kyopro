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
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

int main()
{
    int H,W,Y;
    cin >> H >> W >> Y;
    vvi A(H,vi(W));
    rep(i,0,H){
        rep(j,0,W){
            cin >> A[i][j];
        }
    }
    vvb seen(H,vb(W,false));
    using land = tuple<int,int,int>;
    pq_g<land> lands;
    rep(i,0,H){
        rep(j,0,W){
            if(i==0||i==H-1||j==0||j==W-1){
                lands.push({A[i][j],i,j});
                seen[i][j] = true;
            }
        }
    }
    vi dx = {0,1,0,-1}, dy = {1,0,-1,0};
    int ans = H*W;
    rep(i,1,Y+1){
        while(!lands.empty()&&get<0>(lands.top())<=i){
            land p = lands.top();
            lands.pop();
            ans--;
            int x = get<1>(p);
            int y = get<2>(p);
            rep(j,0,4){
                int xx = x + dx[j];
                int yy = y + dy[j];
                if(!grid_check(xx,yy,H,W)) continue;
                if(seen[xx][yy]) continue;
                lands.push({A[xx][yy],xx,yy});
                seen[xx][yy] = true;
            }
        }
        printf("%d\n",ans);
    }
}