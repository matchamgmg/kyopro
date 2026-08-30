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

int main()
{
    int N;
    cin >> N;
    vector<ll> X(N);
    rep(i,0,N){
        cin >> X[i];
    }
    set<ll> st;
    st.insert(0);
    ll ans = 0;
    rep(i,0,N){
        st.insert(X[i]);
        if(i==0){
            ans = 2*X[i];
        }else{
            auto it = st.find(X[i]);
            auto jt = it; jt++;
            auto kt = it; kt--;
            ll pj, nj, pk, nk, ni;
            if(jt!=st.end()){
                auto jjt = jt; jjt++;
                if(jjt!=st.end()){
                    pj = min((*jt)-(*kt),(*jjt)-(*jt));
                    nj = min((*jt)-(*it),pj);
                }else{
                    pj = (*jt)-(*kt);
                    nj = min((*jt)-(*it),pj);
                }
                ni = min((*jt)-(*it),(*it)-(*kt));
            }else{
                pj = 0; nj = 0; ni = (*it)-(*kt);
            }
            if(kt!=st.begin()){
                auto kkt = kt; kkt--;
                if(jt!=st.end()){
                    pk = min((*kt)-(*kkt),(*jt)-(*kt));
                    nk = min(pk,(*it)-(*kt));
                }else{
                    pk = (*kt)-(*kkt);
                    nk = min(pk,(*it)-(*kt));
                }
            }else{
                pk = (*jt)-(*kt);
                nk = min(pk,(*it)-(*kt));
            }
            ans += (-pj-pk+nj+nk+ni);
        }
        cout << ans << endl;
    }
}