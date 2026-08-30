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
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}

int main()
{
    int H,W,D;
    cin >> H >> W >> D;
    vector<string> S(H);
    rep(i,0,H){
        cin >> S[i];
    }
    int ans = 0;
    rep(i,0,H) rep(j,0,W){
        if(S[i][j]=='#') continue;
        rep(ii,0,H) rep(jj,0,W){
            if(i==ii && j==jj) continue;
            if(S[ii][jj]=='#') continue;
            int score = 0;
            vector<vector<bool>> humid(H,vector<bool>(W,false));
            rep(k,0,H) rep(l,0,W){
                int dist1 = abs(i-k) + abs(j-l);
                int dist2 = abs(ii-k) + abs(jj-l);
                if(humid[k][l]==false && S[k][l]=='.' && (dist1<=D || dist2<=D)){
                    humid[k][l] = true;
                    score++;
                }
            }
            chmax(ans,score);
        }
    }
    cout << ans << endl;
}