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

int main()
{
    int N;
    cin >> N;
    vi A(N);
    rep(i,0,N){
        cin >> A[i];
    }
    vi B1(1e6,-1),B2(1e6,-1);
    rep(i,0,N){
        if(B1[A[i]]==-1){
            B1[A[i]] = i;
        }else if(B2[A[i]]==-1){
            B2[A[i]] = i;
        }else{
            if(B2[A[i]]-B1[A[i]] > i-B2[A[i]]){
                B1[A[i]] = B2[A[i]];
                B2[A[i]] = i;
            }
        }
    }
    int ans = 1e8;
    rep(i,0,B1.size()){
        if(B1[i]!=-1 && B2[i]!=-1){
            chmin(ans,B2[i]-B1[i]+1);
        }
    }
    cout << (ans==1e8 ? -1 : ans) << endl;
}