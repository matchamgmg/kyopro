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
    int T;
    cin >> T;
    rep(i,0,T){
        int N;
        cin >> N;
        vi A(2*N);
        vector position(N,vi(2,-1));
        rep(i,0,2*N){
            cin >> A[i];
            A[i]--;
            if(position[A[i]][0]==-1){
                position[A[i]][0] = i;
            }else{
                position[A[i]][1] = i;
            }
        }
        int ans = 0;
        rep(j,0,N){
            int posi0 = position[j][0];
            int posi1 = position[j][1];
            if(posi0 > posi1) swap(posi0,posi1);
            if(posi1-posi0==1) continue;
            rep(k,0,2){
                int a;
                int a_posi;
                if(k==0){
                    if(posi0-1>=0){
                        a = A[posi0-1];
                        a_posi = posi0-1;
                    }else{
                        continue;
                    }
                }else{
                    if(posi0+1<2*N){
                        a = A[posi0+1];
                        a_posi = posi0+1;
                    }else{
                        continue;
                    }
                }
                rep(l,0,2){
                    int b;
                    int b_posi;
                    if(l==0){
                        if(posi1-1>=0){
                            b = A[posi1-1];
                            b_posi = posi1-1;
                        }else{
                            continue;
                        }
                    }else{
                        if(posi1+1<2*N){
                            b = A[posi1+1];
                            b_posi = posi1+1;
                        }else{
                            continue;
                        }
                    }
                    if(a==b && b_posi-a_posi>1) ans++;
                }
            }
        }
        cout << ans/2 << endl;
    }
}