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
#define input(A) rep(i,0,A.size()) cin >> A[i]

int main()
{
    // 黒マスの区間を管理しようとすると，区間同士のマージが発生して面倒
    // 白マスの区間を管理することにすれば，区間同士のマージは発生しない
    // クエリが進むごとに，白マスは減っていく，区間は細切れになっていく
    // 場合分け ----- : クエリ[L,R]，____ : 白マス[l,r]
    // 1. ----  ____        [l,r] -> [l,r]              何もしない
    // 2. ----===___        [l,r] -> [R+1,r]            クエリに対して高々一回
    // 3. ----======---     [l,r] ->                    複数の区間が消える
    // 4. ____====___       [l,r] -> [l,L-1], [R+1,r]   クエリに対して高々一回
    // 5. ___======----     [l,r] -> [l,L-1]            クエリに対して高々一回
    // 6. ______  -----     [l,r] -> [l,r]              何もしない
    // クエリごとに 2~5 に属する区間をすべて更新する
    // Q個のクエリにおいて区間の数が増える回数は，高々Q回
    // すなわち，区間の数が減少する回数も，高々Q回
    ll N,Q;
    cin >> N >> Q;
    set<pair<ll,ll>> st; // r,l
    ll ans = N;
    st.insert(make_pair(N,1));
    while(Q--){
        ll Li, Ri;
        cin >> Li >> Ri;
        auto itr = st.lower_bound({Li,-1});
        vector<pair<ll,ll>> resv;
        while(itr!=st.end()){
            ll sl = (*itr).second, sr = (*itr).first;
            if(Ri<sl) break;
            if(sl<Li && Ri<sr){
                resv.emplace_back(Li-1,sl);
                resv.emplace_back(sr,Ri+1);
            }else if(Li<=sl && sr<=Ri){

            }else if(sl<Li){
                resv.emplace_back(Li-1,sl);
            }else if(Ri<sr){
                resv.emplace_back(sr,Ri+1);
            }
            // 区間の共通部分が新たに黒マスになる
            ans -= min(sr,Ri) - max(sl,Li) + 1;
            itr = st.erase(itr);
        }
        for(auto p : resv){
            st.insert(p);
        }
        cout << ans << endl;
    }
}