#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
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
template<class T> using pq = priority_queue<T, vector<T>>;//大きい順
template<class T> using pq_g = priority_queue<T, vector<T>, greater<T>>;//小さい順

#define rep(i, s, n) for (int i = s; i < (n); i++)
#define rrep(i, s, n) for (int i = (n) - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n)rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}

ll calc_cost(ll a,ll n){
    return (a*n - n*(n-1)/2);
}

int main()
{
    ll N,M;
    cin >> N >> M;
    ll suma = 0;
    vector<pair<ll,ll>> XA(M);
    rep(i,0,M){
        cin >> XA[i].first;
    }
    rep(i,0,M){
        cin >> XA[i].second;
        suma += XA[i].second;
    }
    if(suma!=N){
        cout << -1 << endl;
        return 0;
    }
    sort(all(XA));
    ll cost = 0;
    rep(i,0,M){
        if(i==0){
            if(XA[i].first>1){
                cout << -1 << endl;
                return 0;
            }
        }
        if(i<M-1){
            if(XA[i].second < XA[i+1].first - XA[i].first){
                cout << -1 << endl;
                return 0;
            }else{
                cost += calc_cost(XA[i].second - 1, min(XA[i+1].first - XA[i].first, XA[i].second - 1));
                XA[i+1].second += XA[i].second - 1 - (XA[i+1].first - XA[i].first - 1);
                XA[i].second -= XA[i+1].first - XA[i].first - 1;
            }
        }else{
            if(XA[i].second < N - XA[i].first + 1){
                cout << -1 << endl;
                return 0;
            }else{
                cost += calc_cost(XA[i].second - 1, N - XA[i].first);
                XA[i].second -= N - XA[i].first;
                if(XA[i].second!=1){
                    cout << -1 << endl;
                    return 0;
                }
            }
        }
    }
    cout << cost << endl;
}