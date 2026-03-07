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
    int Q;
    cin >> Q;
    int N = 500000;
    vector<pair<pair<char,int>,vector<int>>> X(N), Y(N); // Y[i] : char, child_num, child
    Y[0].first = {'.',0};
    X[0].first = {'.',0};
    int Y_siz = 1, X_siz = 1;
    auto add_string_Y = [&](auto f, string &str, int y, int level = 0) -> void {
        if(level == str.size()){
            return;
        }
        char c = str[level];
        bool ok = false;
        for(int nx : Y[y].second){
            if(c==Y[nx].first.first){
                Y[nx].first.second++;
                f(f,str,nx,level+1);
                ok = true;
            }
        }
        if(!ok){
            Y[y].second.push_back(Y_siz);
            Y[Y_siz].first = {c,1};
            Y_siz++;
            f(f,str,Y_siz-1,level+1);
        }
    };
    auto add_string_X = [&](auto f, string &str, int x, int level = 0) -> void {
        if(level == str.size()){
            return;
        }
        char c = str[level];
        bool ok = false;
        for(int nx : X[x].second){
            if(c==X[nx].first.first){
                X[nx].first.second++;
                f(f,str,nx,level+1);
                ok = true;
            }
        }
        if(!ok){
            X[x].second.push_back(X_siz);
            X[X_siz].first = {c,1};
            X_siz++;
            f(f,str,X_siz-1,level+1);
        }
    };
    rep(q,0,Q){
        int t;
        string S;
        cin >> t >> S;
        if(t==1){
            add_string_X(add_string_X,S,0);
        }else{
            add_string_Y(add_string_Y,S,0);
        }
    }
    int ans = 0;
    auto dfs = [&](auto f, int x = 0, int y = 0) -> void {
        if(X[x].second.size()==0){
            ans += Y[y].first.second;
            return;
        }
        for(int nx : X[x].second){
            for(int ny : Y[y].second){
                if(X[nx].first.first==Y[ny].first.first){
                    f(f,nx,ny);
                }
            }
        }
        return;
    };
}