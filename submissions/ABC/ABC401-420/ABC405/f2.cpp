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

int op(int a, int b){ return a + b; }

int e(){ return 0;}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    segtree<int,op,e> seg(2*N+1);
    using S = pair<int,int>;
    vector<S> A(M);    // B,A
    rep(i,0,M){
        cin >> A[i].second >> A[i].first;
    }
    sort(all(A));
    rep(i,0,M){
        seg.set(A[i].second,seg.get(A[i].second) + 1);
    }
    int Q;
    cin >> Q;
    vector<int> ans(Q);
    vector<S> query(Q);
    vector<int> idx(Q);
    iota(all(idx),0);
    rep(i,0,Q){
        cin >> query[i].second >> query[i].first;
    }
    sort(all(idx),[&](int i, int j){ return query[i] < query[j]; });

    int idx_A = 0;
    rep(i,0,Q){
        while(idx_A < M && A[idx_A].first < query[idx[i]].first){
            seg.set(A[idx_A].second, seg.get(A[idx_A].second) - 2);
            seg.set(A[idx_A].first, seg.get(A[idx_A].first) + 1);
            idx_A++;
        }
        ans[idx[i]] = seg.prod(query[idx[i]].second, query[idx[i]].first+1);
    }
    rep(i,0,Q){
        cout << ans[i] << endl;
    }
}