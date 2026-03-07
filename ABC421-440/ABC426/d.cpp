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

void solve(){
    int N;
    string S;
    cin >> N >> S;
    vector<int> left0(N+1,0), left1(N+1,0), right0(N+1,0), right1(N+1,0);
    rep(i,0,N){
        left0[i+1] = left0[i] + 1 + (S[i] == '0');
        left1[i+1] = left1[i] + 1 + (S[i] == '1');
        right0[N-i-1] = right0[N-i] + 1 + (S[N-i-1] == '0');
        right1[N-i-1] = right1[N-i] + 1 + (S[N-i-1] == '1');
    }
    int ans = 3*N;
    int r = 0;
    rep(l,0,N){
        while(r<N && S[l]==S[r]) r++;
        int cnt = 0;
        if(S[l]=='0'){
            cnt = left0[l] + right0[r];
        }else{
            cnt = left1[l] + right1[r];
        }
        chmin(ans,cnt);
    }
    cout << ans << endl;
}

int main()
{
    int T;
    cin >> T;
    while(T--) solve();
}