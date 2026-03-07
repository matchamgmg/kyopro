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

// 盤面の遷移を全探索
// 状態(N桁の二進数,手番)，O(NC2)で探索

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> A(N), B(N);
    rep(i,0,N) cin >> A[i] >> B[i];

    map<pair<ll,bool>,set<pair<ll,bool>>> G;
    map<ll,bool> C;
    queue<pair<ll,bool>> que2;

    ll state = (1LL<<N)-1;
    bool teban = 0;
    queue<pair<ll,bool>> que;
    que.emplace(make_pair(state,teban));
    while(!que.empty()){
        auto p = que.front();
        que.pop();
        ll st = p.first;
        bool t = p.second;
        bool safe = false;
        rep(i,0,N-1){
            rep(j,i+1,N){
                if((~st>>i&1) || (~st>>j&1)) continue;
                if(A[i]==A[j] || B[i]==B[j]){
                    safe = true;
                    ll nst = st - (1LL<<i) - (1LL<<j);
                    if(!G.count({nst,t^1})){
                        que.emplace(make_pair(nst,t^1));
                    }
                    G[{nst,t^1}].emplace(p);
                }
            }
        }
        if(!safe){
            que2.emplace(p);
            C[st] = t;
        }
    }

    // 0: takahashi, 1:aoki
    // if t = 0, exist C[t=1] = 1 -> C[t=0] = 1; else C[t=0] = 0;
    // if t = 1, exist C[t=0] = 0 -> C[t=1] = 0; else C[t=1] = 1;

    while(!que2.empty()){
        auto p = que2.front();
        que2.pop();
        for(auto q : G[p]){
            if(!C.count(q.first)){
                if(q.second == 0){
                    // 0 |= 1
                    C[q.first] = false;
                }else{
                    // 1 &= 0
                    C[q.first] = true;
                }
                que2.emplace(q);
            }
            if(q.second == 0){
                C[q.first] |= C[p.first];
            }else{
                C[q.first] &= C[p.first];
            }
        }
    }

    if(C[state]){
        cout << "Takahashi" << endl;
    }else{
        cout << "Aoki" << endl;
    }
}