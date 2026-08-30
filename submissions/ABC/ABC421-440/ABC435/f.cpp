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

template<class T>
struct CartesianTree {
    int root;
    vector<int> parent, left, right;

    CartesianTree() : root(0) {}
    CartesianTree(const vector<T>& v) :
    root(0), parent(v.size(),-1), left(v.size(),-1), right(v.size(),-1) {
        vector<int> st(v.size(), 0);    // index
        int top = 0; // stack size
        for(int i = 1; i<v.size(); ++i){
            if(v[st[top]] < v[i]){  // pop
                while(top >= 1 && v[st[top-1]] < v[i]) --top;
                left[i] = st[top];  // last popped
                parent[left[i]] = i;
                if(top == 0) {
                    root = i;
                }else{
                    parent[i] = st[top-1];
                    right[parent[i]] = i;
                }
                st[top] = i;
            }else{  // only push
                parent[i] = st[top];
                right[parent[i]] = i;
                st[++top] = i;
            }
        }
    }
};

int main()
{
    // 猫が現在タワーi (1<=i<=N) におり，そこから隣接するタワーへの移動を繰り返して到達できる
    // タワーの範囲が[l,r]である状態を，状態(i,l,r)と定義する
    // 状態(i,l,r)から，運動が終了するまでの猫の移動距離の合計の最大値は，それまでの過程等によらず一意に定まる
    // この値をdp[(i,l,r)]とする
    // 求めたい答えは，高さがNのタワーをk_0として，dp[(k_0,1,N)]

    // 状態(i,l,r)について，次の条件を満たしている必要がある l<=i<-r
    // : max(P[l,r]) = P[i] because 猫が高さkのタワーにいるとき，そのタワーから到達可能なタワーの高さはすべてk以下である
    // 1<=k<=N に対して[L_k,R_k]を，1<=L_k<=k<=R_k<=NかつL_k<=i<=R_kならばP_i<=P_kを満たす極大な区間として定義する
    // 猫がタワーkにいるとき，そこから移動可能な区間の極大が[L_k,R_k]
    // よって，L_i<=l<=i<=r<=R_i を満たす状態(i,l,r)についてのみ考えればよい
    // さらに，(i,L_i,R_i) -> (i,l,r) の遷移が可能なので，dp[(i,L_i,R_i)] >= dp[(i,l,r)]

    // タワーiを撤去することで，直接(i,l,r) -> (j,l',r') に遷移する場合
    // (i,L_i,R_i) から適切に操作を行うことで (j,L_j,R_j) へ遷移可能である
    
    // 状態(i,L_i,R_i) についてのみ考えればよい
    // また，遷移可能なもののうち，タワーiから見て右側の最大M^Rおよび左側の最大M^Lへの遷移だけ考えればよい
    // because そうでないものへの遷移は，M^R，M^Lから遷移可能である
    // Cartesian Tree 上の木DP

    int N;
    cin >> N;
    vi P(N);
    rep(i,0,N) cin >> P[i];

    CartesianTree<int> ct(P);

    auto dfs = [&](auto f, int s) -> ll {
        ll resr = 0, resl = 0;
        if(ct.left[s]!=-1){
            resl = f(f,ct.left[s]) + abs(s-ct.left[s]);
        }
        if(ct.right[s]!=-1){
            resr = f(f,ct.right[s]) + abs(s-ct.right[s]);
        }
        return max(resl,resr);
    };

    int root = 0;
    int mx = 0;
    rep(i,0,N){
        if(P[i]>mx){
            mx = P[i];
            root = i;
        }
    }

    cout << dfs(dfs,root) << endl;
}