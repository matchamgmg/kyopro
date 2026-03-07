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

bool operator< (const pair<ll,vector<ll>> &a, const pair<ll,vector<ll>> &b){
    return a.first < b.first;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, K, X;
    cin >> N >> K >> X;
    vll A(N);
    rep(i,0,N) cin >> A[i];

    // priority_queueで大きいものから調べていくとき，取り出されるものは常に未調査の中で最大のものである
    // 多重集合の管理
    // それぞれの種類の個数をvectorで持って，setで判定

    sort(rall(A));
    ll score = K*A[0];
    vector<ll> state(N,0); state[0] = K;
    pq<pair<ll,vector<ll>>> que;
    set<vector<ll>> seen;
    vector<ll> ans(X);
    que.push(make_pair(score,state));
    ll cnt = 0;
    while(cnt < X && !que.empty()){
        auto p = que.top();
        que.pop();
        if(seen.count(p.second)) continue;
        seen.emplace(p.second);
        ans[cnt] = p.first;
        cnt++;
        rep(i,0,N-1){
            if(p.second[i] == 0) continue;
            vector<ll> nstate = p.second;
            ll nscore = p.first;
            nstate[i]--;
            nstate[i+1]++;
            nscore += A[i+1] - A[i];
            if(seen.count(nstate)) continue;
            que.push(make_pair(nscore,nstate));
        }
    }

    rep(i,0,X){
        cout << ans[i] << endl;
    }
}