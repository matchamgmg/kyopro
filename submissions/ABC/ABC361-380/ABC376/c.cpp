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

bool check(vll &A, vll &B, ll mid,int N){
    vll C = B;
    C.push_back(mid);
    sort(all(C));
    rep(i,0,N){
        if(A[i]>C[i]){
            return false;
        }
    }
    return true;
}

int main()
{
    int N;
    cin >> N;
    vll A(N),B(N-1);
    rep(i,0,N){
        cin >> A[i];
    }
    rep(i,0,N-1){
        cin >> B[i];
    }
    sort(all(A));
    sort(all(B));
    rep(i,0,N-1){
        if(A[i]>B[i]){
            cout << -1 << endl;
            return 0;
        }
    }
    ll l = A[0]-1;
    ll r = A[N-1]+1;
    while(r-l>1){
        ll mid = (l + r)/2;
        if(check(A,B,mid,N)){
            r = mid;
        }else{
            l = mid;
        }
    }
    cout << r << endl;
}