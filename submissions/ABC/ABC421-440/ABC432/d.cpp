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

struct Rectangle {
    ll lx, ly, rx, ry;
};

bool operator<(const Rectangle &a, const Rectangle &b){
    return a.lx < b.lx ? true : a.lx > b.ly ? false :
        a.ly < b.ly ? true : a.ly > b.ly ? false :
        a.rx < b.rx ? true : a.rx > b.rx ? false :
        a.ry < b.ry ? true : false;
}

int main()
{
    ll N, X, Y;
    cin >> N >> X >> Y;
    vector<Rectangle> rec_vec;
    rec_vec.push_back({0,0,X,Y});
    rep(i,0,N){
        char C;
        ll A, B;
        cin >> C >> A >> B;
        vector<Rectangle> old;
        swap(rec_vec,old);
        for(Rectangle a : old){
            if(C=='X'){
                if(a.lx<A && A<a.rx){
                    rec_vec.push_back({a.lx,a.ly-B,A,a.ry-B});
                    rec_vec.push_back({A,a.ly+B,a.rx,a.ry+B});
                }else if(a.rx<=A){
                    rec_vec.push_back({a.lx,a.ly-B,a.rx,a.ry-B});
                }else{
                    rec_vec.push_back({a.lx,a.ly+B,a.rx,a.ry+B});
                }
            }else{
                if(a.ly<A && A<a.ry){
                    rec_vec.push_back({a.lx-B,a.ly,a.rx-B,A});
                    rec_vec.push_back({a.lx+B,A,a.rx+B,a.ry});
                }else if(a.ry<=A){
                    rec_vec.push_back({a.lx-B,a.ly,a.rx-B,a.ry});
                }else{
                    rec_vec.push_back({a.lx+B,a.ly,a.rx+B,a.ry});
                }
            }
        }
    }
    int siz = rec_vec.size();
    vll M(siz);
    rep(i,0,siz){
        M[i] = (rec_vec[i].rx - rec_vec[i].lx)*(rec_vec[i].ry - rec_vec[i].ly);
    }
    dsu uf(siz);
    rep(i,0,siz-1){
        rep(j,i+1,siz){
            Rectangle a = rec_vec[i], b = rec_vec[j];
            ll cx = min(a.rx,b.rx) - max(a.lx,b.lx);
            ll cy = min(a.ry,b.ry) - max(a.ly,b.ly);
            if(cx<0 || cy<0) continue;
            // 0,0を省く
            if(cx || cy) uf.merge(i,j);
        }
    }
    ll ans = 0;
    vll Ans(siz,0);
    rep(i,0,siz){
        if(uf.leader(i)==i) ans++;
        Ans[uf.leader(i)] += M[i];
    }
    sort(all(Ans));
    cout << ans << endl;
    rep(i,0,siz){
        if(Ans[i]!=0) cout << Ans[i] << ' ';
    }
    cout << endl;
}