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

void fn(){
    int N,M;
    cin >> N >> M;
    vi X(N), Y(M);
    rep(i,0,N){
        cin >> X[i];
    }
    rep(j,0,M){
        cin >> Y[j];
    }
    vi Xid(N*M+1,-1), Yid(N*M+1,-1);
    rep(i,0,N){
        if(Xid[X[i]]!=-1){
            cout << "No\n";
            return;
        }
        Xid[X[i]] = i;
    }
    rep(i,0,M){
        if(Yid[Y[i]]!=-1){
            cout << "No\n";
            return;
        }
        Yid[Y[i]] = i;
    }
    // 降順ソートして，おける範囲を拡大していく
    // おける中で最も右下に置くことにすれば実装が楽
    sort(X.rbegin(),X.rend());
    sort(Y.rbegin(),Y.rend());

    vector A(N,vector<int>(M,0));
    vector Ans(N,vector<int>(M,0));

    int xi = 0, yi = 0;
    vector<pair<int,int>> cand;

    rrep(v,1,N*M+1){
        if(xi<N && v==X[xi]){
            rep(j,0,yi){
                cand.emplace_back(xi,j);
            }
            xi++;
        }
        if(yi<M && v==Y[yi]){
            rep(i,0,xi){
                cand.emplace_back(i,yi);
            }
            yi++;
        }
        if(cand.empty()){
            cout << "No\n";
            return;
        }
        auto p = cand.back();
        cand.pop_back();
        A[p.first][p.second] = v;
    }

    rep(i,0,N){
        rep(j,0,M){
            assert(A[i][j]>0);
            Ans[Xid[X[i]]][Yid[Y[j]]] = A[i][j];
        }
    }
    cout << "Yes\n";
    rep(i,0,N){
        rep(j,0,M){
            cout << Ans[i][j] << " \n"[j==M-1];
        }
    }
}

int main()
{
    int T;
    cin >> T;
    rep(i,0,T) fn();
}