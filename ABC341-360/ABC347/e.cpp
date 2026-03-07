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

    int N, Q;
    cin >> N >> Q;
    vector<int> X(Q);
    rep(i,0,Q) cin >> X[i];
    rep(i,0,Q) X[i]--;
    vector<ll> SS(Q+1,0);
    vector<ll> A(N,0);
    vector<int> Ain(N,-1);
    set<int> st;
    rep(i,0,Q){
        if(!st.count(X[i])){
            st.emplace(X[i]);
            Ain[X[i]] = i;
        }else{
            A[X[i]] += SS[i] - SS[Ain[X[i]]];
            Ain[X[i]] = -1;
            st.erase(X[i]);
        }
        SS[i+1] = SS[i] + st.size();
    }
    rep(i,0,N){
        if(Ain[i] >= 0){
            A[i] += SS[Q] - SS[Ain[i]];
        }
    }
    rep(i,0,N){
        cout << A[i] << ' ';
    }
    cout << endl;
}