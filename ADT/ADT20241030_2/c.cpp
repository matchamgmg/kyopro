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
    vi A(N),B(N),C(N);
    vector<bool> used(N,false);
    rep(i,0,N){
        cin >> A[i];
    }
    rep(i,0,N){
        cin >> B[i];
        C[i] = A[i] + B[i];
    }
    
    vi ans;
    rep(cntX,0,X){
        int idx = -1;
        int maxA = -1;
        rep(i,0,N){
            if(used[i]) continue;
            if(maxA < A[i]){
                maxA = A[i];
                idx = i;
            }
        }
        if(idx>=0){
            ans.push_back(idx);
            used[idx] = true;
        }
    }
    rep(cntY,0,Y){
        int idx = -1;
        int maxB = -1;
        rep(i,0,N){
            if(used[i]) continue;
            if(maxB < B[i]){
                maxB = B[i];
                idx = i;
            }
        }
        if(idx>=0){
            ans.push_back(idx);
            used[idx] = true;
        }
    }
    rep(cntZ,0,Z){
        int idx = -1;
        int maxC = -1;
        rep(i,0,N){
            if(used[i]) continue;
            if(maxC < C[i]){
                maxC = C[i];
                idx = i;
            }
        }
        if(idx>=0){
            ans.push_back(idx);
            used[idx] = true;
        }
    }
    sort(all(ans));
    rep(i,0,ans.size()){
        cout << (ans[i]+1) << endl;
    }
}