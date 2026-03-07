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

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    vector<ll> L(M), R(M);
    rep(i,0,M){
        cin >> L[i] >> R[i];
        L[i]--;
    }
    vector<int> idx(M);
    iota(all(idx), 0);
    sort(all(idx), [&](int i, int j){ return L[i]==L[j] ? R[i]<R[j] : L[i]<L[j]; });
    ll covered_right = 0;
    ll range_right = 0;
    int cnt = 0;
    int i = 0;
    bool ok = true;
    while(covered_right<N){
        while(i<M && L[idx[i]]<=covered_right){
            chmax(range_right, R[idx[i]]);
            i++;
        }
        if(covered_right == range_right){
            ok = false;
            break;
        }
        covered_right = range_right;
        cnt++;
    }
    if(covered_right != N) ok = false;
    cout << (ok ? cnt : -1) << endl;
}