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

    int N, K;
    cin >> N >> K;
    vector<vector<int>> A(N, vector<int>(N));
    rep(i,0,N){
        rep(j,0,N) cin >> A[i][j];
    }

    auto f = [&](int i_, int j_) -> bool {
        set<int> color;
        rep(ii,0,2){
            rep(jj,0,2){
                color.emplace(A[i_+ii][j_+jj]);
            }
        }
        return color.size() >= 3;
    };

    int score = 0;
    int result = 0;
    rep(i,0,N-1){
        rep(j,0,N-1){
            score += f(i,j);
        }
    }
    chmax(result, score);

    auto g = [&](int i_, int j_, int c) -> void {
        int before = 0, after = 0;
        int bc = A[i_][j_];
        rep(ii,0,2){
            rep(jj,0,2){
                if(!grid_check(i_-1+ii, j_-1+jj, N-1, N-1)) continue;
                before += f(i_-1+ii, j_-1+jj);
            }
        }
        A[i_][j_] = c;
        rep(ii,0,2){
            rep(jj,0,2){
                if(!grid_check(i_-1+ii, j_-1+jj, N-1, N-1)) continue;
                after += f(i_-1+ii, j_-1+jj);
            }
        }
        A[i_][j_] = bc;

        chmax(result, score - before + after);
        return;
    };

    rep(i,0,N){
        rep(j,0,N){
            set<int> color;
            rep(ii,0,3){
                rep(jj,0,3){
                    if(!grid_check(i-1+ii, j-1+jj, N, N)) continue;
                    color.emplace(A[i-1+ii][j-1+jj]);
                }
            }
            for(int c : color) g(i,j,c);
            if(color.size() < K) g(i,j,0);
        }
    }

    cout << result << endl;
}