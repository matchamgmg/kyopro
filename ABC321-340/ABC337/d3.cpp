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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int H, W, K;
    cin >> H >> W >> K;
    vector<string> S(H);
    rep(i,0,H) cin >> S[i];
    const int INF = 1<<29;
    int ans = INF;
    rep(i,0,H){
        int len = 0;
        int cnt = 0;
        int k = 0;
        rep(j,0,W){
            chmax(k, j);
            if(j>0){
                if(S[i][j-1] == '.'){
                    cnt = max(0, cnt-1);
                    len = max(0, len-1);
                }else if(S[i][j-1] == 'o') len = max(0, len-1);
            }
            while(k<W && S[i][k]!='x' && len<K){
                if(S[i][k] == '.') cnt++;
                len++;
                k++;
            }
            if(len == K) chmin(ans, cnt);
        }
    }
    rep(j,0,W){
        int len = 0;
        int cnt = 0;
        int k = 0;
        rep(i,0,H){
            chmax(k, i);
            if(i>0){
                if(S[i-1][j] == '.'){
                    cnt = max(0, cnt-1);
                    len = max(0, len-1);
                }else if(S[i-1][j] == 'o') len = max(0, len-1);
            }
            while(k<H && S[k][j]!='x' && len<K){
                if(S[k][j] == '.') cnt++;
                len++;
                k++;
            }
            if(len == K) chmin(ans, cnt);
        }
    }
    
    cout << (ans == INF ? -1 : ans) << endl;
}