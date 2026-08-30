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
    vll X(N);
    input(X);
    vll d(N,0);
    set<pair<ll,int>> st;
    st.insert({0,0});
    ll ans = 0;
    rep(i,0,N){
        st.insert({X[i],i+1});
        auto itr = st.find({X[i],i+1});
        auto prv = prev(itr);
        ans -= d[(*prv).second];
        if(prv==st.begin()){
            d[(*prv).second] = (*itr).first - (*prv).first;
        }else{
            d[(*prv).second] = min((*itr).first-(*prv).first,(*prv).first-(*prev(prv)).first);
        }
        ans += d[(*prv).second];
        d[(*itr).second] = (*itr).first - (*prv).first;
        if(next(itr)!=st.end()){
            auto nxt = next(itr);
            ans -= d[(*nxt).second];
            if(next(nxt)==st.end()){
                d[(*nxt).second] = (*nxt).first - (*itr).first;
            }else{
                d[(*nxt).second] = min((*next(nxt)).first-(*nxt).first,(*nxt).first-(*itr).first);
            }
            ans += d[(*nxt).second];
            d[(*itr).second] = min(d[(*itr).second],(*nxt).first-(*itr).first);
        }
        ans += d[(*itr).second];
        cout << ans << '\n';
    }
}