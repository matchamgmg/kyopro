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
    stack<pair<ll,pair<ll,ll>>> st;
    map<ll,ll> color;
    ll a;
    rep(i,0,N){
        cin >> a;
        if(st.empty()){
            st.push(make_pair(a,make_pair(i,i+1)));
            color[a]++;
        }else{
            if(color[a]==0){
                st.push(make_pair(a,make_pair(i,i+1)));
                color[a]++;
            }else{
                while(st.top().first != a){
                    auto p = st.top();
                    color[p.first] -= p.second.second - p.second.first;
                    color[a] += p.second.second - p.second.first;
                    st.pop();
                }
                st.top().second.second = i+1;
                color[a]++;
            }
        }
    }
    vector<ll> ans(N);
    while(!st.empty()){
        auto p = st.top();
        st.pop();
        rep(i,p.second.first,p.second.second){
            ans[i] = p.first;
        }
    }
    rep(i,0,N){
        cout << ans[i] << endl;
    }
}