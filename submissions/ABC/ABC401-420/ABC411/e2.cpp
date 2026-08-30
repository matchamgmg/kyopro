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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<vector<ll>> A(N, vector<ll>(6));
    pq_g<pair<ll,int>> que;
    rep(i,0,N){
        rep(j,0,6){
            cin >> A[i][j];
            que.emplace(A[i][j],i);
        }
    }

    // Nこのさいころを同時に振るとき
    // 最大値が k :
    // すべてk以下　かつ　kを少なくとも一つ含む
    // すべてk以下 - すべてk-1以下
    // すべてk以下である確率:
    // さいころi に関してk以下の面の個数 / 6
    // Aijの昇順に見る
    // 確率の再計算が，ならしO(1)

    vector<int> B(N,0); // k以下の面の個数
    mint pk(0), pk_1(0);
    mint ans(0);

    int zero_cnt = N;

    while(!que.empty()){
        pk_1 = pk;
        auto p = que.top();
        while(!que.empty() && que.top().first == p.first){
            auto q = que.top();
            que.pop();
            if(B[q.second] == 0){
                zero_cnt--;
                B[q.second]++;
                if(zero_cnt == 0){
                    pk = 1;
                    rep(i,0,N){
                        pk *= B[i];
                    }
                }
            }else if(zero_cnt == 0){
                pk *= mint(B[q.second]).inv();
                B[q.second]++;
                pk *= mint(B[q.second]);
            }else{
                B[q.second]++;
            }
        }
        ans += (pk - pk_1)*p.first;
    }
    ans *= mint(6).pow(N).inv();

    cout << ans.val() << endl;
}