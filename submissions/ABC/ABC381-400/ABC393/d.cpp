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
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

#define INF 1LL<<60

int main()
{
    int N;
    cin >> N;
    string S;
    cin >> S;
    vll left(N+1,INF), right(N+1,INF);
    left[0] = 0;
    int j = 0;
    string T = S;
    rep(i,0,N){
        while(j<N && j<i){
            j++;
        }
        while(j<N && T[j]=='1'){
            j++;
        }
        if(j<N && T[j]=='0'){
            left[i+1] = left[i] + j - i;
            swap(T[i],T[j]);
        }
    }
    right[0] = 0;
    j = N-1;
    T = S;
    rep(i,0,N){
        while(j>=0 && j>N-i-1){
            j--;
        }
        while(j>=0 && T[j]=='1'){
            j--;
        }
        if(j>=0 && T[j]=='0'){
            right[i+1] = right[i] - j + N-i-1;
            swap(T[N-i-1],T[j]);
        }
    }
    int cnt = 0;
    rep(i,0,N){
        if(S[i]=='1') cnt++;
    }
    ll ans = INF;
    rep(i,0,N-cnt+1){
        chmin(ans,left[i]+right[N-cnt-i]);
    }
    cout << ans << endl;
}