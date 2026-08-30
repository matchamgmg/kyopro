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

struct S {
    int key;
    int prv, nxt;
    S() : key(-1), prv(-1), nxt(-1) {}
    S(int k) : key(k), prv(-1), nxt(-1) {}
    S(int k, int p, int n) : key(k), prv(p), nxt(n) {}

    bool operator<(const S &rhs) const {
        return key < rhs.key;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> A(N);
    rep(i,0,N) cin >> A[i];
    set<S> st;
    int head = A[0];
    rep(i,0,N){
        int k = A[i];
        int p, n;
        if(i == 0){
            p = -1;
        }else{
            p = A[i-1];
        }
        if(i == N-1){
            n = -1;
        }else{
            n = A[i+1];
        }
        st.emplace(k,p,n);
    }
    int Q;
    cin >> Q;
    int t, x, y;
    for(;Q--;){
        cin >> t;
        if(t == 1){
            cin >> x >> y;
            auto itr = st.lower_bound(S(x));
            S prv = *itr;
            if(prv.nxt == -1){
                st.erase(itr);
                prv.nxt = y;
                st.emplace(prv);
                st.emplace(y,x,-1);
            }else{
                auto itr2 = st.lower_bound(S(prv.nxt));
                S nxt = *itr2;
                prv.nxt = y;
                nxt.prv = y;
                st.erase(itr);
                st.erase(itr2);
                st.emplace(prv);
                st.emplace(nxt);
                st.emplace(y,prv.key,nxt.key);
            }
        }else{
            cin >> x;
            auto itr = st.lower_bound(S(x));
            S h = *itr;
            if(h.prv == -1){
                auto itr2 = st.lower_bound(S(h.nxt));
                S nxt = *itr2;
                st.erase(itr);
                st.erase(itr2);
                st.emplace(nxt.key,-1,nxt.nxt);
                head = nxt.key;
            }else if(h.nxt == -1){
                auto itr2 = st.lower_bound(S(h.prv));
                S prv = *itr2;
                st.erase(itr);
                st.erase(itr2);
                st.emplace(prv.key,prv.prv,-1);
            }else{
                auto itr2 = st.lower_bound(S(h.prv));
                auto itr3 = st.lower_bound(S(h.nxt));
                S prv = *itr2;
                S nxt = *itr3;
                st.erase(itr);
                st.erase(itr2);
                st.erase(itr3);
                st.emplace(prv.key,prv.prv,nxt.key);
                st.emplace(nxt.key,prv.key,nxt.nxt);
            }
        }
    }
    auto itr = st.lower_bound(head);
    while((*itr).nxt != -1){
        cout << (*itr).key << ' ';
        itr = st.lower_bound((*itr).nxt);
    }
    cout << (*itr).key << endl;
}