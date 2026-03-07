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
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

struct Coord {
    int x,y;
};

struct Problem {
    int N,M,K;
    vector<Coord> pos;
    vector<string> v_wall, h_wall;
    int MAX_T;
    Problem() {
        load();
        MAX_T = 2*N*N;
    }
    void load(){
        cin >> N >> M >> K;
        pos.resize(M);
        v_wall.resize(N);
        h_wall.resize(N-1);
        for(auto &x : pos){
            cin >> x.x >> x.y;
        }
        for(auto &x : v_wall){
            cin >> x;
        }
        for(auto &x : h_wall){
            cin >> x;
        }
    }
};

struct Answer {
    int K,M,T,MAX_T;
    vector<vector<char>> buttons;
    vector<int> operations;

    Answer() {}

    Answer(int button_num, int robot_num, int max_t) : K(button_num), M(robot_num), MAX_T(max_t) {
        T = 0;
        buttons.resize(K);
        for(auto &x : buttons){
            x.resize(M);
        }
        operations.resize(MAX_T);
    }
};

struct Solver {
    Problem P;
    Answer ans = Answer(P.K,P.M,P.MAX_T);
};

int main()
{
    Problem prob;
    int a = 0;
}