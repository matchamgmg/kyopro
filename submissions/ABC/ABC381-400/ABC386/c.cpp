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
    int K;
    string S,T;
    cin >> K >> S >> T;
    if(S==T){
        pyes;
        return 0;
    }
    // henkou
    if(S.size()==T.size()){
        int cnt = 0;
        rep(i,0,S.size()){
            if(S[i]!=T[i]){
                cnt++;
            }
        }
        if(cnt>=2){
            pno;
            return 0;
        }else{
            pyes;
            return 0;
        }
    }

    //sounyuu
    if(abs((int)S.size()-(int)T.size())>1){
        pno;
        return 0;
    }
    int cnt = 0;
    int s = 0;
    int t = 0;
    rep(i,0,max(S.size(),T.size())){
        if(S[s]!=T[t]){
            cnt++;
            if(S.size()>T.size()){
                t--;
            }else{
                s--;
            }
        }
        s++;
        t++;
        if(cnt>=2){
            pno;
            return 0;
        }
    }
    pyes;
}