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

int main()
{
    int N,Q;
    cin >> N >> Q;
    int ans = 0;
    int nowl = 0;
    int nowr = 1;
    rep(i,0,Q){
        char H;
        int T;
        cin >> H >> T;
        T--;
        if(H=='L'){
            if(nowl<T){
                if(nowl<nowr&&nowr<T){
                    ans += nowl + N - T;
                }else{
                    ans += T - nowl;
                }
            }else{
                if(T<nowr&&nowr<nowl){
                    ans += T + N - nowl;
                }else{
                    ans += nowl - T;
                }
            }
            nowl = T;
        }else if(H=='R'){
            if(nowr<T){
                if(nowr<nowl&&nowl<T){
                    ans += nowr + N - T;
                }else{
                    ans += T - nowr;
                }
            }else{
                if(T<nowl&&nowl<nowr){
                    ans += T + N - nowr;
                }else{
                    ans += nowr - T;
                }
            }
            nowr = T;
        }
    }
    cout << ans << endl;
}