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
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]
template<class T>
bool chmax(T &a, T b){ if(a < b){ a = b; return true; } else return false; }
template<class T>
bool chmin(T &a, T b){ if(a > b){ a = b; return true; } else return false; }

// クッキーを取り除いた行・列には，クッキーは残っていない
// クッキーが取り除けるかの判定 : その行の幅 >= 2 && その行の残りの枚数 == その行の幅
// サイクルの回数 高々O(H+W)
// サイクルごとの印をつけるかの判定 o(H+W)
// クッキーの削除 合計O(HW)

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int H,W;
    cin >> H >> W;
    vector<string> S(H);
    rep(i,0,H) cin >> S[i];

    vector<vector<int>> tate(26,vector<int>(W,0)), yoko(26,vector<int>(H,0));
    rep(i,0,H){
        rep(j,0,W){
            tate[S[i][j]-'a'][j]++;
            yoko[S[i][j]-'a'][i]++;
        }
    }

    vector<bool> tate_remain(W,true), yoko_remain(H,true);
    int tate_deleted = 0, yoko_deleted = 0;

    queue<tuple<int,int,int>> que;
    int ans = H*W;

    rep(_,0,H+W){
        rep(i,0,W){
            if(tate_remain[i]){
                bool ok = false;
                int c = -1;
                rep(s,0,26){
                    if(tate[s][i] >= 2 && tate[s][i] == H-yoko_deleted){
                        ok |= true;
                        c = s;
                    }
                }
                rep(s,0,26){
                    if(s != c && tate[s][i] > 0){
                        ok = false;
                    }
                }
                if(ok){
                    que.emplace(0,i,c);
                }
            }
        }
        rep(i,0,H){
            if(yoko_remain[i]){
                bool ok = false;
                int c = -1;
                rep(s,0,26){
                    if(yoko[s][i] >= 2 && yoko[s][i] == W-tate_deleted){
                        ok |= true;
                        c = s;
                    }
                }
                rep(s,0,26){
                    if(s != c && yoko[s][i] > 0){
                        ok = false;
                    }
                }
                if(ok){
                    que.emplace(1,i,c);
                }
            }
        }
        while(!que.empty()){
            auto [t,k,c] = que.front();
            que.pop();
            if(t == 0){
                tate_deleted++;
                tate_remain[k] = false;
                rep(i,0,H){
                    if(S[i][k]-'a' == c){
                        tate[c][k]--;
                        yoko[c][i]--;
                        ans--;
                        S[i][k] = '.';
                    }
                }
            }else{
                yoko_deleted++;
                yoko_remain[k] = false;
                rep(i,0,W){
                    if(S[k][i]-'a' == c){
                        tate[c][i]--;
                        yoko[c][k]--;
                        ans--;
                        S[k][i] = '.';
                    }
                }
            }
        }
    }

    rep(i,0,W){
        rep(s,0,26){
            assert(tate[s][i]>=0);
        }
    }
    rep(i,0,H){
        rep(s,0,26){
            assert(yoko[s][i]>=0);
        }
    }

    cout << ans << endl;
}