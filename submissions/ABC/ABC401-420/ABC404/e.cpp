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
    vi C(N-1), A(N);
    rep(i,0,N-1){
        cin >> C[i];
    }
    rep(i,0,N-1){
        cin >> A[i+1];
    }
    vvi G(N);
    const int inf = 1<<29;
    vi dist(N,inf);
    rep(i,0,N-1){
        int e = (i+1)-C[i];
        while(e<i+1){
            G[e].push_back(i+1);
            e++;
        }
    }
    queue<int> q;
    dist[0] = 0;
    q.push(0);
    rep(i,0,N-1){
        if(A[i+1]==1){
            dist[i+1] = 0;
            q.push(i+1);
        }
    }
    while(!q.empty()){
        int p = q.front();
        q.pop();
        for(int n : G[p]){
            if(dist[n]!=inf) continue;
            dist[n] = dist[p] + 1;
            q.push(n);
        }
    }
    int cost = 0;
    rrep(i,1,N){
        if(A[i]==0) continue;
        int e = i - C[i-1];
        int n = -1;
        int min_dist = inf;
        while(e<i){
            if(A[e]>0){
                n = e;
                break;
            }
            e++;
        }
        if(n==-1){
            e = i - C[i-1];
            while(e<i){
                if(dist[e]<min_dist){
                    min_dist = dist[e];
                    n = e;
                }
                e++;
            }
        }
        A[n] += A[i];
        A[i] = 0;
        cost++;
    }
    cout << cost << endl;
}