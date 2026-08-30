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
    rep(i,0,T){
        int N;
        cin >> N;
        vi P(N);
        rep(i,0,N){
            cin >> P[i];
        }
        ll sum_d = 0;
        ll sum_i = 0;
        bool zero_flag = true;
        bool one_flag = false;
        bool two_flag = false;
        rep(i,0,N){
            if(P[i] != i+1){
                zero_flag = false;
            }
        }
        if(zero_flag){
            cout << 0 << endl;
            continue;
        }
        rep(i,0,N){
            sum_d += P[i];
            sum_i += i+1;
            if(sum_d == sum_i && P[i] == i+1){
                one_flag = true;
            }
        }
        if(one_flag){
            cout << 1 << endl;
            continue;
        }
        cout << (P[0]==N&&P[N-1]==1?3:2) << endl;
    }
}