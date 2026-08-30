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

int popcount(int x){
    int y = x;
    int res = 0;
    while(y>0){
        res += y&1;
        y >>= 1;
    }
    return res;
}

int main()
{
    int T;
    cin >> T;
    rep(t,0,T){
        int N;
        cin >> N;
        string S;
        cin >> S;
        // 危険にならないときだけ辺を張る
        vvi G((1<<N));
        for(int i=1; i<(1<<N); i += i){
            if(S[i-1]=='0'){
                G[0].push_back(i);
            }
        }
        rep(i,1,G.size()){
            int pi = popcount(i);
            for(int j=1; j<(1<<N); j+=j){
                if(i+j>=(1<<N)) continue;
                // popcount
                int px = popcount(i+j);
                if(px-pi!=1) continue;
                if(S[i+j-1]=='0'){
                    G[i].push_back(i+j);
                }
            }
        }
        bool ans = false;
        vector<bool> seen((1<<N),false);
        queue<int> que;
        que.push(0);
        seen[0] = true;
        while(!que.empty()){
            int p = que.front();
            que.pop();
            for(int n : G[p]){
                if(seen[n]) continue;
                ans |= (n==((1<<N)-1));
                que.push(n);
                seen[n] = true;
            }
        }
        YN(ans);
    }
}