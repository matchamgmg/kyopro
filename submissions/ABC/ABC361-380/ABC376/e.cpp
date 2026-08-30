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
    int T;
    cin >> T;
    rep(t,0,T){
        int N,K;
        cin >> N >> K;
        vll A(N),B(N);
        rep(i,0,N){
            cin >> A[i];
        }
        rep(i,0,N){
            cin >> B[i];
        }

        vi idx(N);
        iota(all(idx),0);
        sort(all(idx),[&](int i,int j){ return A[i] < A[j]; });
        int r = K;
        pq<ll> que;
        ll ans = 0;
        ll sum_que = 0;
        rep(i,0,r-1){
            que.push(B[idx[i]]);
            sum_que += B[idx[i]];
        }
        ans = A[idx[r-1]]*(sum_que + B[idx[r-1]]);
        rep(i,r+1,N+1){
            //chmin(ans,A[idx[i-1]]*(sum_que + B[idx[i-1]]));
            if(K>1){
                que.push(B[idx[i-2]]);
                sum_que += B[idx[i-2]];
                sum_que -= que.top();
                que.pop();
            }
            chmin(ans,A[idx[i-1]]*(sum_que + B[idx[i-1]]));
        }
        cout << ans << endl;
    }
}