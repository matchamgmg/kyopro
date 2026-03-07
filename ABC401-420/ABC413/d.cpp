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

int main()
{
    int T;
    cin >> T;
    rep(t,0,T){
        int N;
        cin >> N;
        vll A(N);
        vector<pair<ll,bool>> B(N);
        map<ll,int> M;
        rep(i,0,N){
            cin >> A[i];
            B[i] = {abs(A[i]),A[i]>0};
            M[A[i]]++;
        }
        sort(all(A));
        sort(all(B));
        if(N==2){
            cout << "Yes" << endl;
            continue;
        }
        bool ans1 = true;
        rep(i,0,N-2){
            ans1 &= (A[i+1]*A[i+1] == A[i]*A[i+2]);
        }
        bool ans2 = true;
                rep(i,0,N-2){
            ans2 &= ((B[i+1].first*B[i+1].first == B[i].first*B[i+2].first) && B[i].second!=B[i+1].second && B[i].second == B[i+2].second);
        }
        bool ans3 = (M.size()==2 && M[-A[0]]>0 && abs(M[A[0]]-M[-A[0]])<=1);
        YN((ans1||ans2||ans3));
    }
}