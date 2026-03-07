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

// dp
// i番目の区画までをすべて埋めるために必要な最少人数 x

// Interval Set
// T: type of range, VAL: data type
template<class T, class VAL = long long>
struct IntervalSet {
    struct Node {
        T l, r;
        VAL val;
        Node(const T &l, const T &r, const VAL &val) : l(l), r(r), val(val) {}
        constexpr bool operator < (const Node &rhs) const {
            if (l != rhs.l) return l < rhs.l;
            else return r < rhs.r;
        }
        friend ostream& operator << (ostream &s, const Node &e) {
            return s << "([" << e.l << ", " << e.r << "): " << e.val << ")";
        }
    };

    // internal values
    const VAL identity;
    set<Node> S;

    // constructor
    IntervalSet(const VAL &identity = VAL()) : identity(identity) {}
    IntervalSet(const vector<VAL> &v, const VAL &identity = VAL()) : identity(identity) {
        vector<Node> vec;
        for(int l=0; l<(int)v.size();){
            int r = l;
            while(r<(int)v.size() && v[r]==v[l]) r++;
            vec.emplace_back(l,r,v[l]);
            l = r;
        }
        S = set<Node>(vec.begin(), vec.end());
    }

    // get the basic iterators
    constexpr typename set<Node>::iterator begin() { return S.begin(); }
    constexpr typename set<Node>::iterator end() { return S.end(); }

    // get the iterator of interval which contains p
    // not exist -> S.end()
    constexpr typename set<Node>::iterator get(const T &p) {
        auto it = S.upper_bound(Node(p, numeric_limits<T>::max(), 0));
        if (it==S.begin()) return S.end();
        it = prev(it);
        if(it->l<=p && p<it->r) return it;
        else return S.end();
    }

    // get the leftist iterator of interval which contains value >= p
    constexpr typename set<Node>::iterator lower_bound(const T &p) {
        auto it = get(p);
        if(it!=S.end()) return it;
        return S.upper_bound(Node(p, numeric_limits<T>::max(), 0));
    }

    // exist the interval which contains p: true, [l, r): true
    constexpr bool covered(const T &p) {
        auto it = get(p);
        if(it!=S.end()) return true;
        else return false;
    }
    constexpr bool covered(const T &l, const T &r) {
        assert(l<=r);
        if(l==r) return true;
        auto it = get(l);
        if(it!=S.end() && r<=it->r) return true;
        else return false;
    }

    // is p, q in same interval?
    constexpr bool same(const T &p, const T &q) {
        if(!covered(p) || !covered(q)) return false;
        return get(p)==get(q);
    }

    // get the value of interval which contains p
    // not exist -> identity
    constexpr VAL get_val(const T &p) {
        auto it = get(p);
        if(it!=S.end()) return it->val;
        else return identity;
    }
    VAL operator [] (const T &p) const {
        return get_val(p);
    }

    // get mex (>=p)
    constexpr T get_mex(const T &p = 0) {
        auto it = S.upper_bound(Node(p, numeric_limits<T>::max(), 0));
        if(it==S.begin()) return p;
        it = prev(it);
        if(it->l<=p && p<it->r) return it->r;
        else return p;
    }

    // update [l, r) with value val / insert [l, r)
    // del: reflect effects of interval-delete
    // add: reflect effects of interval-add
    // add and del should be reversed operation each other
    template<class ADDFUNC, class DELFUNC>
    void update(T l, T r, const VAL &val, const ADDFUNC &add, const DELFUNC &del) {
        auto it = S.lower_bound(Node(l,0,val));
        while(it!=S.end() && it->l<=r){
            if(it->l==r){
                if(it->val==val){
                    r = it->r;
                    del(it->l, it->r, it->val);
                    it = S.erase(it);
                }
                break;
            }
            if(it->r<=r){
                del(it->l, it->r, it->val);
                it = S.erase(it);
            }else{
                if(it->val==val){
                    r = it->r;
                    del(it->l, it->r, it->val);
                    it = S.erase(it);
                }else{
                    Node node = *it;
                    del(it->l, it->r, it->val);
                    it = S.erase(it);
                    it = S.emplace_hint(it, r, node.r, node.val);
                    add(it->l, it->r, it->val);
                }
            }
        }
        if(it!=S.begin()){
            it = prev(it);
            if(it->r==l){
                if(it->val==val){
                    l = it->l;
                    del(it->l, it->r, it->val);
                    it = S.erase(it);
                }
            }else if(l<it->r){
                if(it->val==val){
                    l = min(l,it->l);
                    r = max(r,it->r);
                    del(it->l, it->r, it->val);
                    it = S.erase(it);
                }else{
                    if(r<it->r){
                        it = S.emplace_hint(next(it), r, it->r, it->val);
                        add(it->l, it->r, it->val);
                        it = prev(it);
                    }
                    Node node = *it;
                    del(it->l, it->r, it->val);
                    it = S.erase(it);
                    it = S.emplace_hint(it, node.l, l, node.val);
                    add(it->l, it->r, it->val);
                }
            }
        }
        if(it!=S.end()) it = next(it);
        it = S.emplace_hint(it, l, r, val);
        add(it->l, it->r, it->val);
    }
    void update(const T &l, const T &r, const VAL &val) {
        update(l, r, val, [](T, T, VAL){}, [](T, T, VAL){});
    }
    template<class ADDFUNC, class DELFUNC>
    void insert(T l, T r, const ADDFUNC &add, const DELFUNC &del) {
        update(l, r, VAL(), add, del);
    }
    void insert(const T &l, const T &r) {
        update(l, r, VAL(), [](T, T, VAL){}, [](T, T, VAL){});
    }

    // erase [l, r)
    // del: reflect effects of interval-delete
    // add: reflect effects of interval-add
    // add and del should be reversed operation each other
    template<class ADDFUNC, class DELFUNC>
    void erase(T l, T r, const ADDFUNC &add, const DELFUNC &del) {
        auto it = S.lower_bound(Node(l, 0, VAL()));
        // COUT(*it);
        while(it!=S.end() && it->l<=r){
            if(it->l==r) break;
            if(it->r<=r){
                del(it->l, it->r, it->val);
                it = S.erase(it);
            }else{
                Node node = *it;
                del(it->l, it->r, it->val);
                it = S.erase(it);
                it = S.emplace_hint(it, r, node.r, node.val);
                add(it->l, it->r, it->val);
            }
        }
        if(it!=S.begin()){
            it = prev(it);
            if(l<it->r){
                if(r<it->r){
                    it = S.emplace_hint(next(it), r, it->r, it->val);
                    add(it->l, it->r, it->val);
                    it = prev(it);
                }
                Node node = *it;
                // COUT(*it);
                del(it->l, it->r, it->val);
                it = S.erase(it);
                it = S.emplace_hint(it, node.l, l, node.val);
                add(it->l, it->r, it->val);
                // COUT(*it);
            }
        }
    }
    void erase(const T &l, const T &r) {
        erase(l, r, [](T, T, VAL){}, [](T, T, VAL){});
    }

    // debug
    friend ostream& operator << (ostream &s, const IntervalSet &ins) {
        for(auto e : ins.S) {
            s << "([" << e.l << ", " << e.r << "): " << e.val << ") ";
        }
        return s;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, M;
    cin >> N >> M;
    vector<ll> L(M), R(M);
    rep(i,0,M){
        cin >> L[i] >> R[i];
        L[i]--;
    }
    vector<int> idx(M);
    iota(all(idx), 0);
    sort(all(idx), [&](int i, int j){ return L[i]==R[i] ? R[i]<R[j] : L[i]<L[j]; });
    IntervalSet<ll,ll> st;
    st.update(-10LL,0LL,0);
    bool ok = true;
    for(int i : idx){
        auto itr = st.get(L[i]-1);
        if(itr == st.end()){
            ok = false;
            break;
        }else{
            if((*itr).r>=R[i]) continue;
            st.update((*itr).r,R[i],(*itr).val+1);
        }
    }
    auto itr = st.get(N-1);
    if(itr == st.end()) ok = false;
    if(ok) cout << (*itr).val << endl;
    else cout << -1 << endl;
}