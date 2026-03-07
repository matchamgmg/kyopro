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

// O(M^N)

// 

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    vector<ll> W(N), C(N);
    rep(i,0,N) cin >> W[i];
    rep(i,0,M) cin >> C[i];
    sort(rall(C));
    vector<ll> dp_k((1<<N),M), dp_rem((1<<N), 0);
    dp_k[0] = 0, dp_rem[0] = C[0];
    rep(bits,0,(1<<N)){
        int k = dp_k[bits];
        ll rem = dp_rem[bits];
        if(k >= M) continue;
        rep(i,0,N){
            if(~bits>>i&1){
                int nbit = (bits | 1<<i);
                int nk = k;
                ll nrem = rem;
                if(rem >= W[i]){
                    nrem -= W[i];
                }else if(k+1<M && C[k+1]>=W[i]){
                    nk++;
                    rem = C[nk] - W[i];
                }else{
                    continue;
                }
                if(nk < dp_k[nbit]){
                    dp_k[nbit] = nk;
                    dp_rem[nbit] = nrem;
                }else if(nk == dp_k[nbit] && nrem > dp_rem[nbit]){
                    dp_rem[nbit] = nrem;
                }
            }
        }
    }
    YN(dp_k[(1<<N)-1]<M);
}