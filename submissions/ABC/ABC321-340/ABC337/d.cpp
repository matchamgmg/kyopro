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

void print(int a,int b,int c){
    printf("%d,%d,%d\n",a,b,c);
}

int main()
{
    int H,W,K;
    cin >> H >> W >> K;
    vector<string> S(H);
    rep(i,0,H){
        cin >> S[i];
    }
    int ans = INF;
    rep(i,0,H){
        if(K>W) break;
        int cnt_o = 0;
        int cnt = 0;
        int last_x = -1;
        rep(j,0,K){
            if(S[i][j]=='x'){
                cnt = 0;
                last_x = j;
            }else if(S[i][j]=='o'){
                cnt_o++;
                cnt++;
            }else{
                cnt++;
            }
        }
        if(cnt>=K){
            chmin(ans,cnt-cnt_o);
        }
//        print(cnt,cnt_o,ans);
        rep(j,K,W){
            if(S[i][j-K]=='o'){
                cnt_o--;
                if(last_x <= j-K) cnt--;
            }else if(S[i][j-K]=='.'){
                if(last_x <= j-K) cnt--;
            }

            if(S[i][j]=='x'){
                cnt = 0;
                last_x = j;
            }else if(S[i][j]=='o'){
                cnt_o++;
                cnt++;
            }else{
                cnt++;
            }
            if(cnt>=K){
                chmin(ans,cnt-cnt_o);
            }
//            print(cnt,cnt_o,ans);
        }
    }

    rep(j,0,W){
        if(K>H) break;
        int cnt_o = 0;
        int cnt = 0;
        int last_x = -1;
        rep(i,0,K){
            if(S[i][j]=='x'){
                cnt = 0;
                last_x = i;
            }else if(S[i][j]=='o'){
                cnt_o++;
                cnt++;
            }else{
                cnt++;
            }
        }
        if(cnt>=K){
            chmin(ans,cnt-cnt_o);
        }
//        print(cnt,cnt_o,ans);
        rep(i,K,H){
            if(S[i-K][j]=='o'){
                cnt_o--;
                if(last_x <= i-K) cnt--;
            }else if(S[i-K][j]=='.'){
                if(last_x <= i-K) cnt--;
            }

            if(S[i][j]=='x'){
                cnt = 0;
                last_x = i;
            }else if(S[i][j]=='o'){
                cnt_o++;
                cnt++;
            }else{
                cnt++;
            }
            if(cnt>=K){
                chmin(ans,cnt-cnt_o);
            }
//            print(cnt,cnt_o,ans);
        }
    }

    cout << (ans==INF ? -1 : ans) << endl;
}