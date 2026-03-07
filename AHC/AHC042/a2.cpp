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
    vector<string> C(N);

    rep(i,0,N){
        cin >> C[i];
    }

    auto move = [&](char c, int n, int r){
        rep(i,0,n){
            cout << c << ' ' << r << endl;
        }

        if(c=='L'){
            rep(i,0,n){
                C[r][i] = '.';
            }
            rep(i,n,N){
                C[r][i-n] = C[r][i];
            }
            rep(i,N-n,N){
                C[r][i] = '.';
            }
        }else if(c=='R'){
            rrep(i,N-n,N){
                C[r][i] = '.';
            }
            rrep(i,0,N-n){
                C[r][i+n] = C[r][i];
            }
            rrep(i,0,n){
                C[r][i] = '.';
            }
        }else if(c=='U'){
            rep(i,0,n){
                C[i][r] = '.';
            }
            rep(i,n,N){
                C[i-n][r] = C[i][r];
            }
            rep(i,N-n,N){
                C[i][r] = '.';
            }
        }else if(c=='D'){
            rrep(i,N-n,N){
                C[i][r] = '.';
            }
            rrep(i,0,N-n){
                C[i+n][r] = C[i][r];
            }
            rrep(i,0,n){
                C[i][r] = '.';
            }
        }
        return;
    };

    rep(i,0,4){
        int r = 2 + i*5;
        rep(j,0,N){
            if(C[j][r]!='x'){
                rep(k,1,3){
                    if(C[j][r+k]=='x'){
                        move('L',k,j);
                        break;
                    }else if(C[j][r-k]=='x'){
                        move('R',k,j);
                        break;
                    }
                }
            }else{
                rep(k,1,3){
                    if(C[j][r+k]=='x'){
                        move('D',1,r+k);
                        break;
                    }else if(C[j][r-k]=='x'){
                        move('D',1,r-k);
                        break;
                    }
                }
            }
        }
        rep(j,0,N){
            if(C[j][r]=='o'){
                if(C[j][r+1]!='o'){
                    move('L',1,j);
                }else if(C[j][r-1]!='o'){
                    move('R',1,j);
                }
            }
        }
        int y_min = 20;
        int y_max = -1;
        rep(j,0,N){
            if(C[j][r]=='x'){
                chmin(y_min,j);
                chmax(y_max,j);
            }
        }
        if(y_min==20 || y_max==-1) continue;
        if(y_min<=N-y_max-1){
            move('U',y_max+1,r);
        }else{
            move('D',N-y_min,r);
        }
    }

    //vv_cout(C);
}