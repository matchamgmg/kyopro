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

#define INF 1<<29

int main()
{
    int N;
    cin >> N;
    string A;
    cin >> A;
    vvi A2;
    vi AA;
    rep(i,0,A.size()){
        AA.push_back(A[i]-'0');
    }
    A2.push_back(AA);
    int n = 1;
    while(n<=N){
        vi AAA;
        for(int i=0;i*3<A2[n-1].size();i++){
            int cnt = 0;
            for(int j=0;j<3;j++){
                cnt += A2[n-1][i*3+j];
            }
            AAA.push_back(cnt>1);
        }
        A2.push_back(AAA);
        n++;
    }
    // vv_cout(A2);
    auto dfs = [&](auto f, int n, int offset, bool toOne) -> int {
        offset *= 3;
        if(n==1){
            int res = 0;
            rep(i,0,3){
                res += A[offset+i]-'0';
            }
            if(toOne){
                return (2-res>0 ? 2-res : 0);
            }else{
                return (res-1>0 ? res-1 : 0);
            }
        }
        int cnt = 0;
        for(int i=0;i<3;i++){
            cnt += A2[n-1][offset+i];
        }
        if(toOne^(cnt<2)){
            return 0;
        }else{
            if(toOne^(cnt%2==0)){
                int res = INF;
                for(int i=0;i<3;i++){
                    int a = f(f,n-1,offset+i,toOne);
                    if(a>0)chmin(res,a);
                }
                return res;
            }else{
                int res = 0;
                int max_res = 0;
                for(int i=0;i<3;i++){
                    int a = f(f,n-1,offset+i,toOne);
                    res += a;
                    chmax(max_res,a);
                }
                res -= max_res;
                return res;
            }
        }
    };
    cout << dfs(dfs,N,0,(A2[N][0]==0)) << endl;
}