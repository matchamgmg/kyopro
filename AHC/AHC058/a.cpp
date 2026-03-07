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
#define input(A) rep(i,0,A.size()) cin >> A[i]

constexpr const ll N = 10, L = 4, T = 500, K = 1;
constexpr const pair<int,int> init_level_id = make_pair(-1,-1);
constexpr const ll INF = 9e18;
array<ll,N> A;
array<array<ll,N>,L> C, Cost;

void input_(){
    ll x;
    cin >> x >> x >> x >> x;
    rep(i,0,N) cin >> A[i];
    rep(i,0,L)rep(j,0,N) cin >> C[i][j];
}

struct State {
    ll t, apple;
    array<array<ll,N>,L> B, P;
    State() : t(0), apple(1) {
        rep(l,0,L){
            rep(i,0,N){
                B[l][i] = 1;
                P[l][i] = 0; 
            }
        }
        rep(l,0,L){
            rep(i,0,N){
                Cost[l][i] = C[l][i]*(P[l][i]+1);
            }
        }
    }

    void enhance(int level, int id){
        apple -= Cost[level][id];
        P[level][id]++;
        Cost[level][id] = C[level][id]*(P[level][id]+1);
    }

    void advance(){
        rep(i,0,N){
            apple += A[i]*B[0][i]*P[0][i];
        }
        rep(l,1,4){
            rep(i,0,N){
                B[l-1][i] += B[l][i]*P[l][i];
            }
        }
        t++;
    }
};

pair<int,int> get_action1(const State &state_){
    int id = -1, level = -1;
    using Score = pair<double,pair<int,int>>;
    priority_queue<Score> que;
    ll income = 0;
    rep(i,0,N){
        income += A[i]*state_.B[0][i]*state_.P[0][i];
    }
    rep(l,0,L){
        rep(i,0,N){
            Score sc;
        }
    }

    return make_pair(level,id);
}

void print_answer(const array<pair<int,int>,T> &answer_){
    for(auto p : answer_){
        if(p==init_level_id) cout << -1 << '\n';
        else cout << p.first << ' ' << p.second << '\n';
    }
}

void solve1(){
    State state;
    pair<int,int> level_id = init_level_id;
    array<pair<int,int>,T> answer;
    while(state.t<T){
        if(level_id==init_level_id){
            level_id = get_action1(state);
        }
        answer[state.t] = level_id;
        if(level_id!=init_level_id && state.apple>=Cost[level_id.first][level_id.second]){
            state.enhance(level_id.first,level_id.second);
            level_id = init_level_id;
        }else{
            answer[state.t] = init_level_id;
        }
        state.advance();
    }
    print_answer(answer);
}

int main()
{
    input_();
    solve1();
}