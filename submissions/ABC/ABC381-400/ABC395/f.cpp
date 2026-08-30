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
    const ll inf = 1LL<<60;
    int N;
    ll X;
    cin >> N >> X;
    vector<pair<ll,int>> U(N), D(N);
    int mu = -1, md = -1;
    ll mlu = inf, mld = inf;
    rep(i,0,N){
        ll u, d;
        cin >> u >> d;
        if(u<mlu){
            mlu = u;
            mu = i;
        }
        if(d<mld){
            mld = d;
            md = i;
        }
        U[i] = {u,i}, D[i] = {d,i};
    }

    ll ans = 0;

    auto f = [&](vector<pair<ll,int>> &t, int m){
        pq_g<pair<ll,int>> q(all(t));
        while(!q.empty()){
            auto p = q.top();
            q.pop();
            if(p.first>t[p.second].first) continue;
            if(p.second-1>=0){
                ll diff = t[p.second-1].first - t[p.second].first;
                if(diff-X>0){
                    ll cost = diff - X;
                    ans += cost;
                    t[p.second-1].first -= cost;
                    q.push(t[p.second-1]);
                }
            }
            if(p.second+1<N){
                ll diff = t[p.second+1].first - t[p.second].first;
                if(diff-X>0){
                    ll cost = diff - X;
                    ans += cost;
                    t[p.second+1].first -= cost;
                    q.push(t[p.second+1]);
                }
            }
        }
    };
    f(U,mu);
    f(D,md);
    
    ll mh = inf;
    ll sum_len = 0;
    rep(i,0,N){
        ll h = U[i].first + D[i].first;
        sum_len += h;
        chmin(mh,h);
    }
    ans += sum_len - N*mh;
    cout << ans << endl;
}