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
    ll A,N;
    cin >> A >> N;
    vector<int> ten(13,0);
    int d = 0;  // 桁数
    ll n = N;
    while(n>0){
        ten[d] = n%10;
        n /= 10;
        d++;
    }
    ll ans = 0;
    rep(t,1,1000001){
        vector<int> x;
        ll y = t;
        int c = 0;
        while(y>0){
            x.push_back(y%10);
            y /= 10;
            c++;
        }
        vector<int> x2 = x;
        reverse(all(x));
        vector<int> xx = x;
        vector<int> xx2 = x;
        rep(i,0,c){
            xx.push_back(x2[i]);
            if(i>0) xx2.push_back(x2[i]);
        }
        ll xxx = 0, xxx2 = 0;
        rep(i,0,2*c){
            xxx *= 10;
            xxx += xx[i];
        }
        rep(i,0,2*c-1){
            xxx2 *= 10;
            xxx2 += xx2[i];
        }
        // A進数
        if(xxx<=N){
            vector<int> aa;
            ll y2 = xxx;
            int c2 = 0;
            while(y2>0){
                aa.push_back(y2%A);
                y2 /= A;
                c2++;
            }
            bool ok1 = true;
            rep(i,0,c2/2){
                ok1 &= (aa[i] == aa[c2-i-1]);
            }
            if(ok1){
                ans += xxx;
            }
        }
        if(xxx2<=N){
            vector<int> aa2;
            ll y3 = xxx2;
            int c3 = 0;
            while(y3>0){
                aa2.push_back(y3%A);
                y3 /= A;
                c3++;
            }
            bool ok2 = true;
            rep(i,0,c3/2){
                ok2 &= (aa2[i] == aa2[c3-i-1]);
            }
            if(ok2){
                ans += xxx2;
            }
        }
    }
    cout << ans << endl;
}