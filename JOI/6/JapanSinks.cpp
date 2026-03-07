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
    int N;
    cin >> N;
    vll A(N);
    rep(i,0,N) cin >> A[i];
    A.emplace_back(-1);
    vector<pair<ll,ll>> B;
    int idx = 0;
    rep(i,0,N){
        if(A[i] != A[i+1]) B.emplace_back(A[i], idx++);
    }
    if(B.size()==1 && B[0].first==0){
        cout << 0 << endl;
        return 0;
    }
    pq_g<pair<ll,ll>> que;
    for(auto p : B){
        que.push(p);
    }
    ll ans = 1;
    ll islands = 1;
    auto f = [&](ll x){
        ll res = 0;
        if(1<=x){
            if(B[x-1].first>B[x].first) res++;
            else res--;
        }else res--;
        if(x<idx-1){
            if(B[x+1].first>B[x].first) res++;
            else res--;
        }else res--;
        return res / 2;
    };
    while(!que.empty()){
        auto p = que.top();
        while(!que.empty() && que.top().first==p.first){
            auto pp = que.top();
            que.pop();
            islands += f(pp.second);
        }
        chmax(ans,islands);
    }
    cout << ans << endl;
}