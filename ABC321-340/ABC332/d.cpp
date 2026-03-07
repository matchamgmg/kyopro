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
    int H,W;
    cin >> H >> W;
    vvi A(H,vi(W)), B(H,vi(W));
    rep(i,0,H){
        rep(j,0,W){
            cin >> A[i][j];
        }
    }
    rep(i,0,H){
        rep(j,0,W){
            cin >> B[i][j];
        }
    }
    map<vvi,int> dist;
    dist[A] = 0;
    queue<vvi> que;
    que.push(A);
    while(!empty(que)){
        vvi A2 = que.front();
        int d = dist[A2];
        que.pop();
        rep(i,0,H-1){
            vvi A3 = A2;
            rep(j,0,W){
                swap(A3[i][j],A3[i+1][j]);
            }
            if(!dist.count(A3)){
                dist[A3] = d + 1;
                que.push(A3);
            }
        }
        rep(i,0,W-1){
            vvi A3 = A2;
            rep(j,0,H){
                swap(A3[j][i],A3[j][i+1]);
            }
            if(!dist.count(A3)){
                dist[A3] = d + 1;
                que.push(A3);
            }
        }
    }
    cout << (!dist.count(B) ? -1 : dist[B]) << endl;
}