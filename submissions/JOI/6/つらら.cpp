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

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, L;
    cin >> N >> L;
    vi A(N+2,0);
    rep(i,1,N+1) cin >> A[i];

    ll time = 0;
    pq_g<pair<ll,int>> que;   // 折れる時刻, id
    rep(i,1,N+1){
        if(A[i-1] < A[i] && A[i] > A[i+1]){
            que.emplace(time+L-A[i], i);
        }
    }
    while(!que.empty()){
        auto p = que.top();
        que.pop();
        chmax(time, p.first);
        A[p.second] = 0;
        if(p.second == 1){
            if(A[p.second] < A[p.second+1] && A[p.second+1] > A[p.second+2]){
                que.emplace(time+L-A[p.second+1], p.second+1);
            }
        }else if(p.second == N){
            if(A[p.second-2] < A[p.second-1] && A[p.second-1] > A[p.second]){
                que.emplace(time+L-A[p.second-1], p.second-1);
            }
        }else{
            if(A[p.second] < A[p.second+1] && A[p.second+1] > A[p.second+2]){
                que.emplace(time+L-A[p.second+1], p.second+1);
            }
            if(A[p.second-2] < A[p.second-1] && A[p.second-1] > A[p.second]){
                que.emplace(time+L-A[p.second-1], p.second-1);
            }
        }
    }
    cout << time << endl;
}