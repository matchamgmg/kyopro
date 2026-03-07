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

int main()
{
    int H,W,X,Y;
    cin >> H >> W >> X >> Y;
    vector<string> S(H);
    rep(i,0,H){
        cin >> S[i];
    }
    string T;
    cin >> T;
    int x = X-1;
    int y = Y-1;
    vector<char> muki = {'U','D','L','R'};
    vector<int> dx = {-1,1,0,0};
    vector<int> dy = {0,0,-1,1};
    vector<vector<bool>> seen(H,vector<bool>(W,false));
    int ans = 0;
    rep(i,0,T.size()){
        rep(j,0,4){
            if(T[i]==muki[j]){
                int xx = x + dx[j];
                int yy = y + dy[j];
                if(S[xx][yy]=='#') continue;
                x = xx;
                y = yy;
                if(S[xx][yy]=='@' && seen[xx][yy]==false) ans++;
                seen[xx][yy] = true;
            }
        }
    }
    x++;
    y++;
    cout << x << " " << y << " " << ans << endl;
}