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
    int N;
    cin >> N;
    vvi A(N,vi(6));
    map<int,vi> num_to_dice;
    set<int> deme;
    rep(i,0,N){
        rep(j,0,6){
            cin >> A[i][j];
            num_to_dice[A[i][j]].push_back(i);
            deme.insert(A[i][j]);
        }
    }
    vi factor(N,0);
    mint xijou(0);
    mint xmiman(0);
    int zeros = N;
    mint ans(0);
    for(int x : deme){
        for(int d : num_to_dice[x]){
            if(factor[d]==0){
                factor[d]++;
                zeros--;
                if(zeros==0){
                    xijou = mint(1);
                    rep(i,0,N){
                        xijou *= factor[i];
                    }
                }
            }else{
                factor[d]++;
                if(zeros==0){
                    xijou *= factor[d];
                    xijou *= mint(factor[d]-1).inv();
                }else{
                    xijou = mint(0);
                }
            }
        }
        mint x_max = xijou - xmiman;
        xmiman = xijou;
        ans += mint(x)*x_max;
    }
    ans *= (mint(6).pow(N)).inv();
    cout << ans.val() << endl;
}