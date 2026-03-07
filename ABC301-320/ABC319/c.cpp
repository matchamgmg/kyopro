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
    vvi C(3,vi(3));
    rep(i,0,3){
        rep(j,0,3){
            cin >> C[i][j];
        }
    }
    
    vi p(9);
    iota(all(p),0);

    auto f = [&](pair<int,set<int>> &line, int num) -> bool {
        if(line.first==2 && line.second.size()==1){
            return false;
        }else{
            line.first++;
            line.second.insert(num);
            return true;
        }
    };

    int all = 0;
    int ok = 0;
    do{
        bool check = true;
        vector<pair<int,set<int>>> scratch(8,{0,set<int>()});
        rep(i,0,9){
            int x = p[i]/3;
            int y = p[i]%3;
            if(x==y){
                check &= f(scratch[6],C[x][y]);
            }
            if(x+y==2){
                check &= f(scratch[7],C[x][y]);
            }
            check &= f(scratch[x],C[x][y]);
            check &= f(scratch[3+y],C[x][y]);
        }
        if(check){
            ok++;
        }
        all++;
    }while(next_permutation(all(p)));

    double ans = (double)ok/all;
    printf("%.10lf\n",ans);
}