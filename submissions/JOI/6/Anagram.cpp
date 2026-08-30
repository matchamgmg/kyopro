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

// EARTH : 5! = 120, E.... : 25~48, EA... : 25~30, EAR.. : 27~28, EARTH = 28
// EEAAT : 5!/2!/2! = 30
// A.... : AAEET, AAETE, AATEE, AEAET, AEATE, AEEAT, AEETA, AETAE, AETEA, ATAEE, ATEAE, ATEEA 

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vll kaijou(21, 1);
    rep(i,0,20) kaijou[i+1] = kaijou[i] * (i+1);

    string S;
    cin >> S;
    vector<int> A(26,0);
    for(char c : S){
        A[c-'A']++;
    }
    int N = S.size();
    ll ans = 0;
    rep(i,0,N){
        char c = S[i];
        rep(j,0,c-'A'){
            // j で始まる文字列の個数
            if(A[j] == 0) continue;
            A[j]--;
            ll num = kaijou[N-(i+1)];
            rep(k,0,26){
                num /= kaijou[A[k]];
            }
            ans += num;
            A[j]++;
        }
        A[c-'A']--;
    }
    cout << ans+1 << endl;
}