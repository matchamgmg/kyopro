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

int lb(const vi &v,int x){
    //先頭からの距離
    return lower_bound(all(v),x) - v.begin();
}

int main()
{
    /*
        [L,R)に対する答えがnであったとする
        長さn以下の11/22文字列はSの[L,R)部分列に含まれている
        長さn+1以上の11/22文字列はS[L,R)部分列に含まれない
        ->単調性
        [L,R)における11/22文字列の最長の長さを二分探索
        m = (l+r)/2として
        長さ2*m+1の11/22文字列が区間内に存在するかの判定をO(1)，O(log N)で行う

    */
    int N,Q;
    cin >> N >> Q;
    string S;
    cin >> S;

    vi idx1,idx2,idxs;
    rep(i,0,N){
        (S[i]=='1' ? idx1
        : S[i]=='2' ? idx2
        : idxs).push_back(i);
    }
    /*
        インデックス : 今までの個数を表す
        値 : 位置を表す
        [L,R)にNこもっているかの判定
        L以降に初めに現れる位置をxとすると
        N個目の位置はidx[x+N-1]
        これがidx[x+N-1] < Rを判定する 
    */

    int L,R;
    rep(i,0,Q){
        cin >> L >> R;
        L--;

        auto P = [&](int len){
            if(len==0){
                // / を発見
                int j = lb(idxs,L);//インデックスを返す
                if(j>=idxs.size()) return false;
                return idxs[j] < R;
            }
            // 1 を発見
            int i = lb(idx1,L);
            if(i+len-1>=idx1.size()) return false;
            // / を発見
            int j = lb(idxs,idx1[i+len-1]+1);
            if(j>=idxs.size()) return false;
            // 2 を発見
            int k = lb(idx2,idxs[j]+1);
            if(k+len-1>=idx2.size()) return false;
            return idx2[k+len-1] < R;
        };

                        //ans = 2*l + 1
        int l = -1;     //l = -1,r = 0のとき，ans = 0（例外）
        int r = N + 1;  //lはかならずN/2以下になる
        while(r-l>1){
            int mid = (l+r)/2;
            (P(mid) ? l : r) = mid;
        }
        cout << (l==-1 ? 0 : 2*l+1) << endl;
    }
}