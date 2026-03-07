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

// 素数ゼータ変換 (約数・倍数ゼータ変換)
// i = 1..N　について
// ans[i] = max_{x|i} (xの倍数であるAの要素数がK以上であるようなx)
// B[x] = xの倍数であるAの要素数

int main()
{
    int N,K;
    cin >> N >> K;
    vi A(N);
    int max_A = 0;
    rep(i,0,N){
        cin >> A[i];
        chmax(max_A,A[i]);
    }
    vi S(max_A+1,0);
    rep(i,0,N){
        S[A[i]]++;
    }
    vi B(max_A+1,0);
    for(int i=1;i<=max_A;i++){
        for(int j=i;j<=max_A;j+=i){     // O(max_A*log max_A)
            B[i] += S[j];
        }
    }
    /* 約数・倍数ゼータ変換
    // for(int p : prime){  // 1 in prime
    //     for(int i=p;i<=max_A;i+=p){
    //         B[p] += S[i];
    //         }
    //     }
    */
    vi Ans(max_A,1);
    for(int i=1;i<=max_A;i++){
        for(int j=i;j<=max_A;j+=i){
            if(B[i]>=K){
                chmax(Ans[j],i);
            }
        }
    }
    rep(i,0,N){
        cout << Ans[A[i]] << '\n';
    }
}