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

#define INF 1<<30

int main()
{
    int M;
    cin >> M;
    vector<string> S(3);
    rep(i,0,3){
        cin >> S[i];
    }
    
    vvb check(10,vb(3,false));
    bool all_check = false;
    rep(i,0,10){
        rep(j,0,3){
            rep(k,0,M){
                if(S[j][k]==('0'+i)){
                    check[i][j] = true;
                }
            }
        }
        all_check |= (check[i][0]&&check[i][1]&&check[i][2]);
    }
    if(!all_check){
        cout << -1 << endl;
        return 0;
    }

    auto slot = [&](int n, int i, int time, int s) -> int {
        int c = 0;
        if(n!=1){
            c++;
        }
        int m = M;
        while(m>0 && S[s][(time+c)%M]!=('0'+i)){
            c++;
            m--;
        }
        if(m==0){
            return -1;
        }
        return c;
    };

    int ans = INF;
    rep(i,0,10){
        rep(j1,0,3){
            rep(j2,0,3){
                if(j2==j1) continue;
                rep(j3,0,3){
                    if(j3==j1||j3==j2) continue;
                    int t = 0;
                    int cnt = 0;
                    cnt = slot(1,i,t,j1);
                    if(cnt==-1){
                        break;
                    }
                    t += cnt;
                    cnt = slot(2,i,t,j2);
                    if(cnt==-1){
                        break;
                    }
                    t += cnt;
                    cnt = slot(3,i,t,j3);
                    if(cnt==-1){
                        break;
                    }
                    t += cnt;
                    chmin(ans,t);
                }
            }
        }
    }
    if(ans==INF){
        ans = -1;
    }
    cout << ans << endl;
}