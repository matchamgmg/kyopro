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
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]
template<class T>
bool chmax(T &a, T b){ if(a < b){ a = b; return true; } else return false; }
template<class T>
bool chmin(T &a, T b){ if(a > b){ a = b; return true; } else return false; }

void solve(){
    int N;
    cin >> N;
    vector<int> R(N);
    rep(i,0,N) cin >> R[i];
    rep(i,0,N) R[i]--;
    vector<int> lowest(N, N);
    pq_g<pair<int,int>> que;
    rep(i,0,N){
        que.emplace(R[i],i);
    }
    while(!que.empty()){
        auto p = que.top();
        que.pop();
        if(lowest[p.second] < p.first) continue;
        chmin(lowest[p.second],p.first);
        int next_l = p.first+1;
        if(p.second-1>=0){
            if(lowest[p.second-1] > next_l){
                chmin(lowest[p.second-1],next_l);
                que.emplace(lowest[p.second-1],p.second-1);
            }
        }
        if(p.second+1<N){
            if(lowest[p.second+1] > next_l){
                chmin(lowest[p.second+1], next_l);
                que.emplace(lowest[p.second+1],p.second+1);
            }
        }
    }
    ll cost = 0;
    rep(i,0,N){
        cost += abs(lowest[i]-R[i]);
    }
    cout << cost << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    for(;T--;) solve();
}