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
    int N,M,Sx,Sy;
    cin >> N >> M >> Sx >> Sy;
    map<ll,set<ll>> xhouse,yhouse;
    rep(i,0,N){
        int X,Y;
        cin >> X >> Y;
        xhouse[X].insert(Y);
        yhouse[Y].insert(X);
    }
    vector<char> muki = {'U','D','L','R'};
    vll dx = {0LL,0LL,-1LL,1LL};
    vll dy = {1LL,-1LL,0,0};
    ll x = Sx;
    ll y = Sy;
    rep(i,0,M){
        char D;
        ll C;
        cin >> D >> C;
        rep(j,0,4){
            if(D==muki[j]){
                ll xx = x + dx[j]*C;
                ll yy = y + dy[j]*C;
                if(j==0){
                    while(xhouse[xx].upper_bound(yy) != xhouse[xx].lower_bound(y)){
                        int yyy = *xhouse[xx].lower_bound(y);
                        xhouse[xx].erase(yyy);
                        yhouse[yyy].erase(xx);
                    }
                }else if(j==1){
                    while(xhouse[xx].upper_bound(y) != xhouse[xx].lower_bound(yy)){
                        int yyy = *xhouse[xx].lower_bound(yy);
                        xhouse[xx].erase(yyy);
                        yhouse[yyy].erase(xx);
                    }
                }else if(j==2){
                    while(yhouse[yy].upper_bound(x) != yhouse[yy].lower_bound(xx)){
                        int xxx = *yhouse[yy].lower_bound(xx);
                        yhouse[yy].erase(xxx);
                        xhouse[xxx].erase(yy);
                    }
                }else if(j==3){
                    while(yhouse[yy].upper_bound(xx) != yhouse[yy].lower_bound(x)){
                        int xxx = *yhouse[yy].lower_bound(x);
                        yhouse[yy].erase(xxx);
                        xhouse[xxx].erase(yy);
                    }
                }
                x = xx;
                y = yy;
            }
        }
    }
    int ans = N;
    for(auto k : xhouse){
        ans -= k.second.size();
    }
    cout << x << " " << y << " " << ans << endl;
}