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

constexpr const int N = 20, M = 40;

struct XY {
    int x, y;
};

struct Problem {
    array<XY,M> p;

    void load(){
        int n, m;
        cin >> n >> m;
        rep(i,0,M){
            cin >> p[i].x >> p[i].y;
        }
    }
};

struct Solver {
    Problem p;
    XY xy_now;
    vector<pair<char,char>> ans;
    Solver(){
        p.load();
        xy_now = p.p[0];
    }
    void print_ans(){
        rep(i,0,ans.size()){
            cout << ans[i].first << ' ' << ans[i].second << '\n';
        }
        cout << flush;
    }
    void always_move(XY goal){
        int dx = goal.x - xy_now.x;
        int dy = goal.y - xy_now.y;
        xy_now.x += dx;
        xy_now.y += dy;
        if(dx>=0){
            while(dx>0){
                dx--;
                ans.push_back({'M','D'});
            }
        }else{
            while(dx<0){
                dx++;
                ans.push_back({'M','U'});
            }
        }
        if(dy>=0){
            while(dy>0){
                dy--;
                ans.push_back({'M','R'});
            }
        }else{
            while(dy<0){
                dy++;
                ans.push_back({'M','L'});
            }
        }
    }
    void always_move_or_skate(XY goal){
        int dx = goal.x - xy_now.x;
        int dy = goal.y - xy_now.y;
        xy_now.x += dx;
        xy_now.y += dy;
        if(dx>=0){
            int s = N-1 - goal.x;
            if(s+1 <= dx){
                ans.push_back({'S','D'});
                while(s>0){
                    s--;
                    ans.push_back({'M','U'});
                }
            }else{
                while(dx>0){
                    dx--;
                    ans.push_back({'M','D'});
                }
            }
        }else{
            int s = -goal.x;
            if(s-1 >= dx){
                ans.push_back({'S','U'});
                while(s<0){
                    s++;
                    ans.push_back({'M','D'});
                }
            }else{
                while(dx<0){
                    dx++;
                    ans.push_back({'M','U'});
                }
            }
        }
        if(dy>=0){
            int s = N-1 - goal.y;
            if(s+1 <= dy){
                ans.push_back({'S','R'});
                while(s>0){
                    s--;
                    ans.push_back({'M','L'});
                }
            }else{
                while(dy>0){
                    dy--;
                    ans.push_back({'M','R'});
                }
            }
        }else{
            int s = -goal.y;
            if(s-1 >= dy){
                ans.push_back({'S','L'});
                while(s<0){
                    s++;
                    ans.push_back({'M','R'});
                }
            }else{
                while(dy<0){
                    dy++;
                    ans.push_back({'M','L'});
                }
            }
        }

    }
    void solve(){
        rep(i,1,M){
            always_move_or_skate(p.p[i]);
        }
    }
};

int main()
{
    Solver solver;
    solver.solve();
    solver.print_ans();
}