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

vector<int> Z_algorithm(string &S){
    int N = S.size();
    vector<int> prefix(N,0);
    prefix[0] = N;
    // i: 窓の先頭
    // j: prefix一致数
    int i = 1, j = 0;
    while(i<N){
        // prefixの一致数をカウントアップして記録する（0または途中から）
        while(i+j<N && S[j]==S[i+j]) j++;
        prefix[i] = j;
        if(j==0){
            i++;
            continue;
        }
        // prefix arrayの値を利用する
        int k = 1;
        while(i+k<N && k+prefix[k]<j){
            prefix[i+k] = prefix[k];
            k++;
        }
        // カウントアップの初期値を再設定（jの値）
        i += k;
        j -= k;
    }
    return prefix;
}

int main()
{
    int T;
    cin >> T;
    while(T--){
        string A, B;
        cin >> A >> B;
        int n = A.size();
        string BdAA = B + "$" + A + A;
        vi Z = Z_algorithm(BdAA);
        int ans = -1;
        rep(i,n+1,n+n+1){
            if(Z[i]==n){
                ans = i-n-1;
                break;
            }
        }
        cout << ans << endl;
    }
}