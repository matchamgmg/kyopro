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

int main()
{
    int N;
    cin >> N;
    vi L(N),R(N);
    rep(i,0,N){
        cin >> L[i] >> R[i];
    }
    vi idx(N);
    iota(all(idx),0);
    sort(all(idx),[&](int i, int j){ return L[i] < L[j]; });

    ll ans = 0;
    // l_i == i_j
    int prev_l = -1;
    int cmb_l = 0;
    rep(i,0,N){
        if(L[idx[i]]!=prev_l){
            prev_l = L[idx[i]];
            cmb_l = 1;
        }else{
            ans += cmb_l;
            cmb_l++;
        }
    }
    // l_i < l_j
    rep(i,0,N){
        int ul = -1, ur = N;
        auto P = [&](int m, int x) -> bool {
            return L[idx[m]] <= x;
        };
        while(ur-ul>1){
            int mid = (ul+ur)/2;
            (P(mid,L[idx[i]]) ? ul : ur) = mid;
        }
        int l = ul;
        ul = -1; ur = N;
        while(ur-ul>1){
            int mid = (ul+ur)/2;
            (P(mid,R[idx[i]]) ? ul : ur) = mid;
        }
        int r = ul;
        ans += r - l;
    }
    cout << ans << endl;
}