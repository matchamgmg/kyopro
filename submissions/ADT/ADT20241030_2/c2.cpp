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
    int N,X,Y,Z;
    cin >> N >> X >> Y >> Z;
    vi A(N),B(N);
    rep(i,0,N){
        cin >> A[i];
    }
    rep(i,0,N){
        cin >> B[i];
    }

    /*
    (1,A1,B1),...,(N,AN,BN) について
    1. 全体を(-Ai,i)の昇順にソート
    2. X+1番目以降を(-Bi,i)の昇順にソート
    3. X+Y+1番目以降を(-(Ai+Bi),i)の昇順にソート
    4. X+Y+Z番目以前をiの昇順にソート

    添え字の配列をソートすることを考える
    (-Ai,i)の昇順にソート == iの昇順にソートしてからAiの降順に安定ソート
    */

    vi S(N);
    iota(all(S),0);
    stable_sort(all(S),[&](int i,int j){return A[i] > A[j];});

    sort(S.begin()+X,S.end());
    stable_sort(S.begin()+X,S.end(),[&](int i,int j){return B[i] > B[j];});

    sort(S.begin()+X+Y,S.end());
    stable_sort(S.begin()+X+Y,S.end(),[&](int i,int j){return A[i]+B[i] > A[j]+B[j];});

    sort(S.begin(),S.begin()+X+Y+Z);

    rep(i,0,X+Y+Z){
        cout << (S[i]+1) << endl;
    }
}