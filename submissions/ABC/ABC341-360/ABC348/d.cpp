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

using point = pair<int,int>;
using medicine = pair<point,int>;

int main()
{
    int H,W;
    cin >> H >> W;
    point S,T;
    vector<string> A(H);
    rep(i,0,H){
        cin >> A[i];
        rep(j,0,W){
            if(A[i][j]=='S') S = {i,j};
            if(A[i][j]=='T') T = {i,j};
        }
    }
    int N;
    cin >> N;
    vector<medicine> med(N);
    bool check1 = false;
    bool check2 = false;
    int s = -1;
    int g = -1;
    rep(i,0,N){
        cin >> med[i].first.first >> med[i].first.second >> med[i].second;
        med[i].first.first--;
        med[i].first.second--;
        if(med[i].first.first==T.first && med[i].first.second==T.second){
            check1 = true;
            g = i;
        }
        if(med[i].first.first==S.first && med[i].first.second==S.second){
            check2 = true;
            s = i;
        }
    }
    if(!check1){
        med.push_back({T,0});
        g = N;
    }
    if(!check2){
        pno;
        return 0;
    }

    vvi G((int)med.size());
    vi dx = {0,1,0,-1}, dy = {1,0,-1,0};
    auto bfs1 = [&](int i_){
        medicine s_ = med[i_];
        vvi en(H,vi(W,-1));
        en[s_.first.first][s_.first.second] = s_.second;
        queue<medicine> que;
        que.push(s_);
        while(!que.empty()){
            medicine now = que.front();
            que.pop();
            if(now.second<=0) continue;
            rep(i,0,4){
                medicine next;
                next.first.first = now.first.first + dx[i];
                next.first.second = now.first.second + dy[i];
                next.second = now.second - 1;
                if(!grid_check(next.first.first,next.first.second,H,W)) continue;
                if(A[next.first.first][next.first.second]=='#') continue;
                if(en[next.first.first][next.first.second]>=next.second) continue;
                en[next.first.first][next.first.second] = next.second;
                que.push(next);
            }
        }
        rep(i,0,med.size()){
            if(en[med[i].first.first][med[i].first.second]>=0){
                G[i_].push_back(i);
            }
        }
        return;
    };

    rep(i,0,med.size()){
        bfs1(i);
    }

    auto bfs2 = [&](int s_, int g_) -> bool {
        vi dist((int)med.size(),1<<29);
        dist[s_] = 0;
        queue<int> que;
        que.push(s_);
        while(!que.empty()){
            int now = que.front();
            int d = dist[now];
            que.pop();
            for(int next : G[now]){
                if(dist[next]<=d+1) continue;
                dist[next] = d + 1;
                que.push(next);
            }
        }
        return dist[g_] < (1<<29);
    };
    if(bfs2(s,g)){
        pyes;
    }else{
        pno;
    }
}