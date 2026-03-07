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
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}

int main()
{
    int H,W,Q;
    cin >> H >> W >> Q;
    vector<set<int>> tate(W), yoko(H);
    rep(i,0,H) rep(j,0,W){
        tate[j].insert(i);
        yoko[i].insert(j);
    }
    vector<vector<bool>> wall(H,vector<bool>(W,true));
    int ans = H*W;
    rep(i,0,Q){
        int R,C;
        cin >> R >> C;
        R--; C--;
        if(wall[R][C]){
            ans--;
            wall[R][C] = false;
            tate[C].erase(R);
            yoko[R].erase(C);
        }else{
            auto right_itr = yoko[R].lower_bound(C);
            auto top_itr = tate[C].lower_bound(R);
            int right = W, left = -1, top = H, bottom = -1;
            if(right_itr!=yoko[R].end()){
                right = *right_itr;
            }
            if(top_itr!=tate[C].end()){
                top = *top_itr;
            }
            if(right_itr!=yoko[R].begin()){
                left = *--right_itr;
            }
            if(top_itr!=tate[C].begin()){
                bottom = *--top_itr;
            }
            if(right!=W){
                yoko[R].erase(right);
                tate[right].erase(R);
                ans--;
                wall[R][right] = false;
            }
            if(top!=H){
                yoko[top].erase(C);
                tate[C].erase(top);
                ans--;
                wall[top][C] = false;
            }
            if(left!=-1){
                yoko[R].erase(left);
                tate[left].erase(R);
                ans--;
                wall[R][left] = false;
            }
            if(bottom!=-1){
                yoko[bottom].erase(C);
                tate[C].erase(bottom);
                ans--;
                wall[bottom][C] = false;
            }
        }
    }
    cout << ans << endl;
}