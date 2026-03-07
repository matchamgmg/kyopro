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
    vvi A(N);
    vi K(N);
    rep(i,0,N){
        cin >> K[i];
        rep(j,0,K[i]){
            int a;
            cin >> a;
            A[i].push_back(a);
        }
        sort(all(A[i]));
    }

    auto calc = [&](int i, int j) -> long double{
        int siz_i = A[i].size();
        int siz_j = A[j].size();
        if(siz_i>siz_j){
            swap(i,j);
            swap(siz_i,siz_j);
        }
        ll ct = 0;
        ll cnt = 0;
        int idx_j = 0;
        int idx_i = 0;
        for(;idx_i<siz_i;idx_i++){
            int a = A[i][idx_i];
            if(idx_i>0 && A[i][idx_i-1]==a){
                ct += cnt;
                continue;
            }else{
                cnt = 0;
            }
            while(idx_j<siz_j && A[j][idx_j]<a){
                idx_j++;
            }
            while(idx_j<siz_j && A[j][idx_j]==a){
                cnt++;
                idx_j++;
            }
            ct += cnt;
        }
        return (long double)ct / K[i] / K[j];
    };

    long double ans = -1;
    rep(i,0,N-1){
        rep(j,i+1,N){
            chmax(ans,calc(i,j));
        }
    }
    printf("%.12Lf\n",ans);
}