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
    map<ll,vector<vector<ll>>> keisu;
    rep(i,0,N){
        ll a = A[i] % 3;
        ll b = A[i] % 5;
        ll c = A[i] % 7;
        if(a==0){
            ll d = A[i] / 3;
            if(keisu[d].size()==0){
                keisu[d] = vector<vector<ll>>(3,vector<ll>());
                keisu[d][0].emplace_back(i);
            }else{
                keisu[d][0].emplace_back(i);
            }
        }
        if(b==0){
            ll d = A[i] / 5;
            if(keisu[d].size()==0){
                keisu[d] = vector<vector<ll>>(3,vector<ll>());
                keisu[d][1].emplace_back(i);
            }else{
                keisu[d][1].emplace_back(i);
            }
        }
        if(c==0){
            ll d = A[i] / 7;
            if(keisu[d].size()==0){
                keisu[d] = vector<vector<ll>>(3,vector<ll>());
                keisu[d][2].emplace_back(i);
            }else{
                keisu[d][2].emplace_back(i);
            }
        }
    }
    ll ans = 0;
    for(auto &p : keisu){
        for(ll i : p.second[2]){
            auto itrk = lower_bound(all(p.second[0]),i);
            ll rightk = itrk - p.second[0].begin();
            auto itrkk = lower_bound(rall(p.second[0]),i,[](ll x, ll y){ return x > y; });
            ll leftk = itrkk - p.second[0].rbegin();
            auto itrj = lower_bound(all(p.second[1]),i);
            ll rightj = itrj - p.second[1].begin();
            auto itrjj = lower_bound(rall(p.second[1]),i,[](ll x, ll y){ return x > y; });
            ll leftj = itrjj - p.second[1].rbegin();
            ans += leftk*rightj + leftj*rightk;
        }
        for(ll k : p.second[0]){
            auto itri = lower_bound(all(p.second[2]),k);
            ll righti = itri - p.second[2].begin();
            auto itrii = lower_bound(rall(p.second[2]),k,[](ll x, ll y){ return x > y; });
            ll lefti = itrii - p.second[2].rbegin();
            auto itrj = lower_bound(all(p.second[1]),k);
            ll rightj = itrj - p.second[1].begin();
            auto itrjj = lower_bound(rall(p.second[1]),k,[](ll x, ll y){ return x > y; });
            ll leftj = itrjj - p.second[1].rbegin();
            ans += lefti*rightj + leftj*righti;
        }
    }
    cout << ans << endl;
}