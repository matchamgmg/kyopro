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

// 状態を表す変数を考える
// 状態 : li, ri, lj, rj, ti, tj
// -> O(H^3*W^3)

int main()
{
    int H,W;
    cin >> H >> W;
    vector<string> S(H);
    rep(i,0,H){
        cin >> S[i];
    }
    int si, sj;
    rep(i,0,H){
        rep(j,0,W){
            if(S[i][j] == 'T'){
                si = i, sj = j;
            }
        }
    }
    using State = vector<int>;
    map<State,int> dist;
    queue<State> que;

    auto push = [&](int li, int ri, int lj, int rj, int ti, int tj, int d){
        // 新しい状態を作る
        li = max(li,ti-si); ri = min(ri,H+(ti-si));
        lj = max(lj,tj-sj); rj = min(rj,W+(tj-sj));
        if(li<=ti && ti<ri && lj<=tj && tj<rj){
            if(S[ti][tj]=='#') return;
        }
        State s = {li,ri,lj,rj,ti,tj};
        if(dist.count(s)) return;
        dist[s] = d;
        que.push(s);
    };

    int ans = -1;

    push(0,H,0,W,si,sj,0);
    while(!que.empty()){
        State u = que.front();
        que.pop();
        int d = dist[u];
        //ごみの個数が0の状態が見つかったら終了
        int cnt = 0;
        {
            rep(i,u[0],u[1]){
                rep(j,u[2],u[3]){
                    cnt += (S[i][j] == '#');
                }
            }
        }
        if(cnt == 0){
            ans = d;
            break;
        }

        push(u[0],u[1],u[2],u[3],u[4]+1,u[5],d+1);
        push(u[0],u[1],u[2],u[3],u[4]-1,u[5],d+1);
        push(u[0],u[1],u[2],u[3],u[4],u[5]+1,d+1);
        push(u[0],u[1],u[2],u[3],u[4],u[5]-1,d+1);
    }
    cout << ans << endl;
}