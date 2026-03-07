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
    int H,W;
    cin >> H >> W;
    ll X;
    cin >> X;
    int P,Q;
    cin >> P >> Q;
    P--; Q--;
    vvll S(H,vll(W));
    vector<vector<bool>> taka(H,vector<bool>(W,false));
    rep(i,0,H){
        rep(j,0,W){
            cin >> S[i][j];
        }
    }
    pq_g<tuple<ll,int,int>> slime;
    ll takahashi = S[P][Q];
    taka[P][Q] = true;
    vi dx = {1,0,-1,0}, dy = {0,1,0,-1};
    rep(i,0,4){
        int x = P + dx[i];
        int y = Q + dy[i];
        if(0<=x && x<H && 0<=y && y<W){
            slime.push({S[x][y],x,y});
            taka[x][y] = true;
        }
    }
    while(!slime.empty()){
        ll s;
        int x,y;
        s = get<0>(slime.top());
        x = get<1>(slime.top());
        y = get<2>(slime.top());
        slime.pop();
        if((double)takahashi/X<=s) break;
        takahashi += s;
        rep(i,0,4){
            int xx = x + dx[i];
            int yy = y + dy[i];
            if(0<=xx && xx<H && 0<=yy && yy<W && taka[xx][yy]==false){
                slime.push({S[xx][yy],xx,yy});
                taka[xx][yy] = true;
            }
        }
    }
    cout << takahashi << endl;
}