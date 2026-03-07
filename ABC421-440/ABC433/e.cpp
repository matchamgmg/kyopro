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
        X[i]--;
    }
    rep(j,0,M){
        cin >> Y[j];
        Y[j]--;
    }
    // N行M列の行列の各要素は1以上NM以下で相異なる
    // 各行の最大値がX ... Xを少なくともひとつ，かつすべてX以下
    // 各列の最大値がY
    // Xがl個以上 ... i行目にXを置く
    // 相異なるという条件がなければ各マスにmin(X[i],Y[j])を書けばよい
    // NMから1まで順番に埋めていく
    // 大きい方から埋めていくと，進むにつれて候補地点は単調増加
    vi invX(N*M,-1), invY(N*M,-1);
    rep(i,0,N){
        if(invX[X[i]]!=-1){
            cout << "No" << endl;
            return;
        }else{
            invX[X[i]] = i;
        }
    }
    rep(j,0,M){
        if(invY[Y[j]]!=-1){
            cout << "No" << endl;
            return;
        }else{
            invY[Y[j]] = j;
        }
    }
    vector<vector<pair<int,int>>> A(N*M);
    rep(i,0,N)rep(j,0,M){
        A[min(X[i],Y[j])].emplace_back(i,j);
    }
    queue<pair<int,int>> waiting;
    vector<vector<int>> Ans(N,vector<int>(M,0));
    int v = M*N-1;
    while(v>=0){
        if(invX[v]!=-1 && invY[v]!=-1){
            Ans[invX[v]][invY[v]] = v+1;
            for(auto p : A[v]){
                if(p == make_pair(invX[v],invY[v])) continue;
                waiting.push(p);
            }
        }else if(invX[v]==-1 && invY[v]==-1){
            if(waiting.empty()){
                cout << "No" << endl;
                return;
            }
            auto p = waiting.front();
            waiting.pop();
            Ans[p.first][p.second] = v+1;
            for(auto q : A[v]){
                waiting.push(q);
            }
        }else{
            if(A[v].empty()){
                cout << "No" << endl;
                return;
            }
            auto p = A[v].back();
            A[v].pop_back();
            Ans[p.first][p.second] = v+1;
            for(auto q : A[v]){
                waiting.push(q);
            }
        }
        v--;
    }
    cout << "Yes" << endl;
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
    while(T--){
        fn();
    }
}