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
    int a,b;
    ll C;
    cin >> a >> b >> C;
    // X,Yをそれぞれ桁ごとに見た時の値の組(1,1),(0,0),(1,0),(0,1)
    // n10 + n01 + n11 + n00 = 60
    // a = n10 + n11
    // b = n01 + n11
    // c = popcount(C) = n01 + n10
    // a - b = n10 - n01
    // n10 = (a-b+c)/2 >= 0
    // n01 = (c-a+b)/2 >= 0
    // n11 = a - n10 = (a+b-c)/2 >= 0
    // n00 = 60 - n10 - n01 - n11 = 60 - (a+b+c)/2 >= 0
    int c = 0;
    int d = 0;
    while(d<60){
        if((C>>d)&1) c++;
        d++;
    }
    if(a-b+c < 0 || c-a+b < 0 || a+b-c < 0 || a+b+c > 120 || (a+b+c)%2==1){
        cout << -1 << endl;
        return 0;
    }
    vector<ll> X(60,0),Y(60,0);
    ll x = 0, y = 0;
    int n10 = (a-b+c)/2, n11 = (a+b-c)/2, n01 = (c-a+b)/2, n00 = 60 - (a+b+c)/2;
    for(int i = 0;(1LL<<i) < (1LL<<60);i++){
        if((C>>i)&1){
            if(n10>0){
                n10--;
                X[i] = 1;
            }else{
                n01--;
                Y[i] = 1;
            }
        }else{
            if(n11>0){
                n11--;
                X[i] = 1;
                Y[i] = 1;
            }else{
                n00--;
            }
        }
        x += X[i]<<i;
        y += Y[i]<<i;
    }
    cout << x << ' ' << y << endl;
}