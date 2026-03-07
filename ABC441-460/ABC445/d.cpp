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

// 縦横どちらかの最大値は，長方形の縦横に一致

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll H, W, N;
    cin >> H  >> W >> N;
    vector<ll> h(N), w(N);
    ll hmax = 0, wmax = 0;
    rep(i,0,N){
        cin >> h[i] >> w[i];
        chmax(hmax, h[i]);
        chmax(wmax, w[i]);
    }
    pq<pair<ll,int>> tate, yoko;
    rep(i,0,N){
        tate.emplace(h[i],i);
        yoko.emplace(w[i],i);
    }
    vector<bool> used(N, false);
    vector<pair<int,int>> ans(N);
    ll x = 0, y = 0;
    int cnt = 0;
    if(hmax == H){
        auto p = tate.top();
        tate.pop();
        used[p.second] = true;
        ans[p.second].first = x;
        ans[p.second].second = y;
        W -= w[p.second];
        x += w[p.second];
        cnt++;
    }
    while(cnt < N){
        while(!yoko.empty() && used[yoko.top().second] == true) yoko.pop();
        if(!yoko.empty() && yoko.top().first == W){
            auto p = yoko.top();
            yoko.pop();
            used[p.second] = true;
            ans[p.second].first = x;
            ans[p.second].second = y;
            H -= h[p.second];
            y += h[p.second];
            cnt++;
        }
        while(!tate.empty() && used[tate.top().second] == true) tate.pop();
        if(!tate.empty() && tate.top().first == H){
            auto p = tate.top();
            tate.pop();
            used[p.second] = true;
            ans[p.second].first = x;
            ans[p.second].second = y;
            W -= w[p.second];
            x += w[p.second];
            cnt++;
        }
    }
    rep(i,0,N){
        cout << ans[i].second+1 << ' ' << ans[i].first+1 << endl;
    }
}