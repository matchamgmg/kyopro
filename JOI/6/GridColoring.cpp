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
    vi A(N), B(N);
    rep(i,0,N) cin >> A[i];
    rep(i,0,N) cin >> B[i];

    map<int,int> count_color;
    rep(i,0,N){
        count_color[A[i]]++;
        count_color[B[i]]++;
    }
    count_color[A[0]]--;

    rep(i,1,N-1){
        if(A[i+1] < A[i]) A[i+1] = A[i];
        if(B[i+1] < B[i]) B[i+1] = B[i];
    }

    pq_g<pair<int,int>> que;
    rep(i,1,N) que.emplace(A[i],0);
    rep(i,1,N) que.emplace(B[i],1);

    ll height = 0, width = 0;
    ll ans_area = 0;
    ll ans_color = 0;
    ll area = 0;
    while(!que.empty()){
        auto p = que.top();
        while(!que.empty() && que.top().first == p.first){
            auto q = que.top();
            que.pop();
            if(q.second == 0) height++;
            else width++;
        }
        ll narea = width * height;
        if(narea - area + count_color[p.first] >= ans_area){
            ans_area = narea - area + count_color[p.first];
            ans_color = p.first;
        }
        area = narea;
    }

    cout << ans_color << ' ' << ans_area << endl;
}