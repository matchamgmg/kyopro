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

// 偏角ソート
// 外積を用いる
// [0, 180), [180, 360)に分ける

ll cross(ll x1, ll y1, ll x2, ll y2){
    return x1*y2 - x2*y1;
}

bool comp(pair<ll,ll> &a, pair<ll,ll> &b){
    ll ah = (a.second < 0 or (a.second == 0 and a.first < 0));
    ll bh = (b.second < 0 or (b.second == 0 and b.first < 0));
    if(ah != bh) return ah < bh;
    return cross(a.first,a.second,b.first,b.second) > 0;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;
    vector<pair<ll,ll>> P(N);
    rep(i,0,N){
        cin >> P[i].first >> P[i].second;
    }
    vi idx(N);
    iota(all(idx),0);
    sort(all(idx),[&](int i, int j){ return comp(P[i],P[j]); });

    vi dict(N);
    rep(i,0,N){
        dict[idx[i]] = i;
    }

    vi R(N), L(N);
    L[0] = 0;
    rep(i,1,N){
        if(not comp(P[idx[i-1]],P[idx[i]])) L[i] = L[i-1];
        else L[i] = i;
    }

    R[N-1] = N-1;
    rrep(i,0,N-1){
        if(not comp(P[idx[i]],P[idx[i+1]])) R[i] = R[i+1];
        else R[i] = i;
    }

    int A, B;
    rep(q,0,Q){
        cin >> A >> B;
        A--; B--;
        int apos = dict[A];
        int bpos = dict[B];
        if(R[apos] >= L[bpos]) cout << R[apos] - L[bpos] + 1 << endl;
        else cout << N - (L[bpos] - R[apos] - 1) << endl;
    }
}