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

// 同じ色への移動はコスト1
// (start, goal) = (b, b), (r, r) : cost = 1
// (b, r), (r, b) :
// if goalが内陸 && startが内陸 : cost = 3
// else if どちらか一方が内陸 : cost = 2
// else どちらも沿岸 : cost = 1, 2

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;
    string S;
    cin >> S;
    set<int> inner;
    rep(i,0,N){
        if(S[(i+N-1)%N] == S[i] && S[i] == S[(i+1)%N]) inner.emplace(i);
    }

    int X, Y;
    while(Q--){
        cin >> X >> Y;
        X--; Y--;
        int ans = 0;
        if((X+1)%N == Y || (Y+1)%N == X) ans = 1;
        else if(S[X] == S[Y]) ans = 1;
        else if(inner.count(X) && inner.count(Y)) ans = 3;
        else ans = 2;
        cout << ans << endl;
    }
}