#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
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

#define rep(i, s, n) for (int i = s; i < (n); i++)
#define rrep(i, s, n) for (int i = (n) - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n)rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}

struct UnionFind
{
    vector<int> par, siz, score;
    /*
    par[i] : iを含むグループの根
    siz[i] : iを根とする木の頂点数
    */

    //初期化
    UnionFind(int n) : par(n, -1), siz(n, 1), score(n,-100'000) {}

    //根を求める
    int root(int x){
        if(par[x]==-1) return x;
        else return par[x] = root(par[x]);
    }

    //xとyが同じグループに属するかどうか
    bool issame(int x, int y){
        return root(x) == root(y);
    }

    //xを含むグループとyを含むグループとを併合する
    bool unite(int x, int y){
        x = root(x); y = root(y);
        if(x==y) return false;
        if(siz[x] < siz[y]) swap(x,y);  //分岐を書くのを省略しているだけ
        par[y] = x;
        siz[x] += siz[y];
        score[x] += score[y];
        return true;
    }

    //連結成分の個数を求める
    int groups(){
        int res = 0;
        for(int i=0;i<(int)par.size();i++){
            if(root(i)==i) res++;
        }
        return res;
    }

    //xを含むグループのサイズを返す
    int size(int x){
        return siz[root(x)];
    }
};

int main()
{
    int N;
    cin >> N;
    vector<pair<int,int>> saba(N), iwashi(N);
    rep(i,0,N){
        cin >> saba[i].first >> saba[i].second;
    }
    rep(i,0,N){
        cin >> iwashi[i].first >> iwashi[i].second;
    }
    //100*100のブロック*10^6に分ける
    int field_size = 100'000;
    int block_size = 100;
    int block_num = (field_size + block_size - 1)/block_size;
    vvi blocks(block_num,vi(block_num,0));
    rep(i,0,N){
        int x = saba[i].first / block_size;
        int y = saba[i].second / block_size;
        blocks[x][y]++;
    }
    rep(i,0,N){
        int x = iwashi[i].first / block_size;
        int y = iwashi[i].second / block_size;
        blocks[x][y]--;
    }
    priority_queue<pair<int,pair<int,int>>> que;
    rep(i,0,block_num){
        rep(j,0,block_num){
            que.push({blocks[i][j],{i,j}});
        }
    }
    //上位Mブロックを取り出して併合
    int M = 2000;
    UnionFind uf(block_num*block_num);
    set<pair<int,int>> s;
    vi dx = {1,0,-1,0}, dy = {0,1,0,-1};//右下左上
    rep(i,0,M){
        auto e = que.top();
        que.pop();
        int x = e.second.first;
        int y = e.second.second;
        int score = e.first;
        uf.score[y*block_num + x] = score;
        if(!s.empty()){
            for(auto p : s){
                rep(j,0,4){
                    if(x+dx[j]==p.first && y+dy[j]==p.second){
                        uf.unite(y*block_num + x, p.second*block_num + p.first);
                    }
                }
            }
        }
        s.insert({x,y});
    }
    //もっともスコアの高いユニオンを取り出す
    int best_union = -1;
    int best_score = -100000;
    rep(i,0,block_num*block_num){
        if(i!=uf.root(i)) continue;
        if(best_score < uf.score[i]){
            best_score = uf.score[i];
            best_union = i;
        }
    }
    //ユニオンに含まれるブロック番号をすべて取り出す
    set<pair<int,int>> set2;
    int bx = best_union%block_num;
    int by = best_union/block_num;
    int edge_x = bx;
    int edge_y = by;//左上の頂点
    set2.insert({bx,by});
    rep(i,0,block_num*block_num){
        if(i==best_union) continue;
        if(uf.root(i)==best_union){
            bx = i%block_num;
            by = i/block_num;
            set2.insert({bx,by});
            if(bx+by<=edge_x+edge_y){
                edge_x = bx;
                edge_y = by;
            }
        }
    }
    //ユニオンの外周の頂点を求める
    int direction = 0;//向き
    int nx = edge_x;
    int ny = edge_y;
    vector<pair<int,int>> ans;
    //ans.push_back({edge_x, edge_y});
    do {
        //自分の立っている場所は関係ない
        //自身の四方だけ見る
        //1ターンに必ず1進む
        if(direction==0){
            if(set2.count({nx+dx[(direction+4-1)%4], ny+dy[(direction+4-1)%4]})){
                ans.push_back({nx,ny});
                direction = (direction+4-1)%4;
                nx += dx[direction];
                ny += dy[direction];
            }else if(set2.count({nx+dx[direction], ny+dy[direction]})){
                nx += dx[direction];
                ny += dy[direction];
            }else{
                nx += dx[direction];
                ny += dy[direction];
                ans.push_back({nx,ny});
                direction = (direction+1)%4;
            }
        }else if(direction==1){
            if(set2.count({nx+dx[direction], ny+dy[direction]})){
                nx += dx[direction];
                ny += dy[direction];
                ans.push_back({nx,ny});
                direction = (direction+4-1)%4;
            }else if(set2.count({nx+dx[(direction+1)%4], ny+dy[(direction+1)%4]})){
                nx += dx[direction];
                ny += dy[direction];
            }else{
                ans.push_back({nx,ny});
                direction = (direction+1)%4;
                nx += dx[direction];
                ny += dy[direction];
            }
        }else if(direction==2){
            if(set2.count({nx+dx[direction], ny+dy[direction]})){
                ans.push_back({nx,ny});
                direction = (direction+4-1)%4;
                nx += dx[direction];
                ny += dy[direction];
            }else if(set2.count({nx+dx[(direction+1)%4]+dx[direction], ny+dy[(direction+1)%4]+dy[direction]})){
                nx += dx[direction];
                ny += dy[direction];
            }else{
                ans.push_back({nx,ny});
                direction = (direction+1)%4;
                nx += dx[direction];
                ny += dy[direction];
            }
        }else if(direction==3){
            if(set2.count({nx+dx[direction]+dx[(direction+4-1)%4], ny+dy[direction]+dy[(direction+4-1)%4]})){
                ans.push_back({nx,ny});
                direction = (direction+4-1)%4;
                nx += dx[direction];
                ny += dy[direction];
            }else if(set2.count({nx+dx[direction], ny+dy[direction]})){
                nx += dx[direction];
                ny += dy[direction];
            }else{
                ans.push_back({nx,ny});
                direction = (direction+1)%4;
                nx += dx[direction];
                ny += dy[direction];
            }
        }
    } while(nx!=edge_x || ny!=edge_y);
    if(ans.back().first!=edge_x || ans.back().second!=edge_y){
        ans.push_back({nx,ny});
    }
    cout << ans.size() << endl;
    rep(i,0,ans.size()){
        cout << (ans[i].first*block_size) << " " << (ans[i].second*block_size) << endl;
    }
    /*
    auto e = que.top();
    cout << 4 << endl;
    int x = e.second.first;
    int y = e.second.second;
    cout << x << " " << y << endl;
    cout << (x+block_size) << " " << y << endl;
    cout << (x+block_size) << " " << (y+block_size) << endl;
    cout << x << " " << (y+block_size) << endl;
    */
}