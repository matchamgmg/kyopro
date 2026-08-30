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

int main()
{
    // 乗算のオーバーフローに気を付けよう
    int N;
    cin >> N;
    vll A(N);
    input(A);
    sort(all(A));
    ll ans_all = (ll)N*(N-1)*(N-2)/6;
    ll ans_dif = 0, ans_same = 0;
    ll i = 0, k = 0;
    rep(j,0,N){
        while(i<j && A[i]<A[j]) i++;
        while(k<N && A[k]<=A[j]) k++;
        ans_same += (j-i)*(k-1-j);
        ans_dif += i*(N-k);
    }
    cout << ans_all - ans_dif - ans_same << endl;

    // ll ans_b = 0;
    // vector<pair<int,int>> run_length;
    // pair<int,int> last = {-1,0};
    // rep(i,0,N){
    //     if(last.first==A[i]){
    //         last.second++;
    //     }else{
    //         if(last.first!=-1){
    //             run_length.emplace_back(last);
    //         }
    //         last = {A[i],1};
    //     }
    // }
    // run_length.emplace_back(last);
    // for(auto p : run_length){
    //     ans_b += p.second*(p.second-1)/2 * (N-p.second);
    // }
    // cout << ans_b << endl;

    // ll ans_c = 0;
    // map<int,int> mp;
    // rep(i,0,N){
    //     mp[A[i]]++;
    // }
    // for(auto [u,v] : mp){
    //     ans_c += v*(v-1)*(N-v)/2;
    // }
    // cout << ans_c << endl;
}