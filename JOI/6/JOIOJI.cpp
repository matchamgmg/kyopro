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

// 区間[l,r)にJOIが(r-l)/3文字ずつ含まれる
// 2文字のとき, P[i] := [0,i) における Jの個数-Iの個数
// このとき，Jの個数 == Iの個数となる区間は P[l] == P[r] をみたす [l, r)
// 3文字のとき，JOI J-O, O-I を管理したときに
// P[l] == P[r] && Q[l] == Q[r] を満たす最大の[l,r)

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    string S;
    cin >> S;

    vector<int> J(N+1,0), O(N+1,0), I(N+1,0);
    rep(i,0,N){
        J[i+1] = J[i] + (S[i] == 'J');
        O[i+1] = O[i] + (S[i] == 'O');
        I[i+1] = I[i] + (S[i] == 'I');
    }
    vector<int> P(N+1,0), Q(N+1,0);
    map<pair<int,int>, vector<int>> mp;
    rep(i,0,N+1){
        P[i] = J[i] - O[i];
        Q[i] = O[i] - I[i];
        mp[make_pair(P[i],Q[i])].emplace_back(i);
    }

    int ans = 0;
    for(auto p : mp){
        auto q = minmax_element(all(p.second));
        chmax(ans, (*q.second - *q.first));
    }
    cout << ans << endl;
}