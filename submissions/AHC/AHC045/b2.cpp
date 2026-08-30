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

constexpr const int N = 800;
constexpr const int Q = 400;
constexpr const int M_max = 400;
constexpr const int GridNum = 20;
constexpr const int GridSize = 500;

class TimeKeeper{
    private:
    std::chrono::high_resolution_clock::time_point start_time_;
    int64_t time_threshold_;

    public:
    TimeKeeper(){};

    TimeKeeper(const int64_t &time_threshold)
    : start_time_(std::chrono::high_resolution_clock::now()),
      time_threshold_(time_threshold)
    {}

    bool isTimeOver() const{
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
        auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;

        return duration_cast<milliseconds>(diff).count() >= time_threshold_;
    }

    auto get_time() const{
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
        auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;
        return duration_cast<milliseconds>(diff).count();
    }
};

struct Area{
    int lx, rx, ly, ry;
};

struct Point{
    int x, y;
};

struct Edge{
    int x, y;
    double len;
};

bool operator<(const Edge &a, const Edge &b){
    if(a.len<b.len) return true;
    else if(a.len>b.len) return false;
    else{
        if(a.x<b.x) return true;
        else if(a.x>b.x) return false;
        else{
            if(a.y<b.y) return true;
            else return false;
        }
    }
}

bool operator>(const Edge &a, const Edge &b){
    if(a.len>b.len) return true;
    else if(a.len<b.len) return false;
    else{
        if(a.x>b.x) return true;
        else if(a.x<b.x) return false;
        else{
            if(a.y>b.y) return true;
            else return false;
        }
    }
}

bool operator==(const Edge &a, const Edge &b){
    return (a.len==b.len && a.x==b.x && a.y==b.y);
}

struct Problem{
    int n,M,q,L,W;
    array<int,M_max> G;
    array<Area,N> R;

    void load(){
        cin >> n >> M >> q >> L >> W;
        rep(i,0,M) cin >> G[i];
        for(Area &a : R){
            cin >> a.lx >> a.rx >> a.ly >> a.ry;
        }
    }
};

struct Solver{
    TimeKeeper tk;
    mt19937 mt;
    Problem p;
    array<Point,N> center;
    Solver(){
        mt = mt19937(0);
        p.load();
        tk = TimeKeeper(500);
        rep(i,0,N){
            center[i].x = (p.R[i].lx + p.R[i].rx)/2;
            center[i].y = (p.R[i].ly + p.R[i].ry)/2;
        }
    }

    void print_ans(vector<set<int>> &groups, vector<vector<pair<int,int>>> &trees){
        cout << "!\n";
        rep(i,0,p.M){
            for(auto &g : groups[i]) cout << g << ' ';
            cout << '\n';
            rep(j,0,p.G[i]-1){
                cout << trees[i][j].first << ' ' << trees[i][j].second << '\n';
            }
        }
        cout << flush;
    }

    double calc_dist(int i, int j){
        return sqrt((center[i].x-center[j].x)*(center[i].x-center[j].x)+(center[i].y-center[j].y)*(center[i].y-center[j].y));
    }

    double struct_MST(set<int> &group, vector<pair<int,int>> &tree){
        double score = 0;
        dsu uf(N);
        pq_g<Edge> que;
        int i = 0;
        for(int u : group){
            int j = 0;
            for(int v : group){
                if(j<=i){
                    j++;
                    continue;
                }
                double d = calc_dist(u,v);
                que.push({u,v,d});
                j++;
            }
            i++;
        }
        while(!que.empty()){
            auto [x,y,d] = que.top();
            que.pop();
            if(!uf.same(x,y)){
                uf.merge(x,y);
                tree.push_back({x,y});
                score += d;
            }
        }
        return score;
    }

    vector<pair<int,int>> query(vector<int> &group){
        int siz = group.size();
        cout << "? " << siz << ' ';
        rep(i,0,siz) cout << group[i] << " \n"[i==siz-1];
        cout << flush;
        vector<pair<int,int>> res(siz-1);
        rep(i,0,siz-1){
            cin >> res[i].first >> res[i].second;
        }
        return res;
    }

    void reconst_MST(set<int> &group, vector<pair<int,int>> &tree, vector<vector<pair<int,int>>> &query_tree){
        dsu uf(N);
        int idx_tree = 0;
        for(auto t : query_tree){
            for(auto [a,b] : t){
                uf.merge(a,b);
                tree[idx_tree] = {a,b};
                idx_tree++;
            }
        }

        pq_g<Edge> que;
        int i = 0;
        for(int u : group){
            int j = 0;
            for(int v : group){
                if(j<=i){
                    j++;
                    continue;
                }
                double d = calc_dist(u,v);
                que.push({u,v,d});
                j++;
            }
            i++;
        }

        while(!que.empty()){
            auto [x,y,d] = que.top();
            que.pop();
            if(!uf.same(x,y)){
                uf.merge(x,y);
                tree[idx_tree] = {x,y};
                idx_tree++;
            }
        }
        return;
    }

    void solve(){
        vector groups(p.M, set<int>());
        vector trees(p.M, vector<pair<int,int>>());
        array<int,N> belong;
        // グリッドに分割する
        vector grid(GridNum,vector<set<int>>(GridNum,set<int>()));
        rep(i,0,N){
            int a = center[i].x/GridSize;
            int b = center[i].y/GridSize;
            if(center[i].x==10000) a = GridNum-1;
            if(center[i].y==10000) b = GridNum-1;
            grid[a][b].insert(i);
        }
        // 初期解を作る
        queue<int> que;
        rep(i,0,GridNum){
            if(i%2==0){
                rep(j,0,GridNum){
                    for(int u : grid[i][j]) que.push(u);
                }
            }else{
                rrep(j,0,GridNum){
                    for(int u : grid[i][j]) que.push(u);
                }
            }
        }


        array<double,M_max> ave_x, ave_y, var_x, var_y, var_xy;
        rep(i,0,p.M){
            ave_x[i] = 0;
            ave_y[i] = 0;
            var_x[i] = 0;
            var_y[i] = 0;
            var_xy[i] = 0;
        }

        int v;
        rep(i,0,p.M){
            // 初期解を構築する
            int cnt = 0;
            while(cnt<p.G[i]){
                v = que.front();
                que.pop();
                belong[v] = i;
                cnt++;
                groups[i].insert(v);
                ave_x[i] += center[v].x;
                ave_y[i] += center[v].y;
                var_x[i] += center[v].x*center[v].x;
                var_y[i] += center[v].y*center[v].y;
                var_xy[i] += center[v].x*center[v].y;
            }
            // 各グループの平均・分散・共分散を求める
            ave_x[i] /= cnt;
            ave_y[i] /= cnt;
            var_x[i] = var_x[i]/cnt - ave_x[i]*ave_x[i];
            var_y[i] = var_y[i]/cnt - ave_y[i]*ave_y[i];
            var_xy[i] = var_xy[i]/cnt - ave_x[i]*ave_y[i];
        }

        // 山登り
        if(p.M>1){
            while(!tk.isTimeOver()){
                [&](){
                    int u,v;
                    u = mt()%N;
                    do{
                        v = mt()%N;
                    }while(belong[v]==belong[u]);
    
                    // 差分更新
                    int i = belong[u], j = belong[v];
                    double psum = var_x[i] + var_y[i] + abs(var_xy[i])
                                + var_x[j] + var_y[j] + abs(var_xy[j]);
                    
                    int diff_x = center[v].x - center[u].x;
                    int diff_y = center[v].y - center[u].y;
                    int diff_xx = center[v].x*center[v].x - center[u].x*center[u].x;
                    int diff_yy = center[v].y*center[v].y - center[u].y*center[u].y;
                    int diff_xy = center[v].x*center[v].y - center[u].x*center[u].y;
    
                    double navei_x = (ave_x[i] * p.G[i] + diff_x) / p.G[i];
                    double navej_x = (ave_x[j] * p.G[j] - diff_x) / p.G[j];
                    double navei_y = (ave_y[i] * p.G[i] + diff_y) / p.G[i];
                    double navej_y = (ave_y[j] * p.G[j] - diff_y) / p.G[j];
    
                    double nvari_x = ((var_x[i] + ave_x[i]*ave_x[i]) * p.G[i] + diff_xx) / p.G[i] - navei_x*navei_x;
                    double nvarj_x = ((var_x[j] + ave_x[j]*ave_x[j]) * p.G[j] - diff_xx) / p.G[j] - navej_x*navej_x;
                    double nvari_y = ((var_y[i] + ave_y[i]*ave_y[i]) * p.G[i] + diff_yy) / p.G[i] - navei_y*navei_y;
                    double nvarj_y = ((var_y[j] + ave_y[j]*ave_y[j]) * p.G[j] - diff_yy) / p.G[j] - navej_y*navej_y;
                    double nvari_xy = ((var_xy[i] + ave_x[i]*ave_y[i]) * p.G[i] + diff_xy) / p.G[i] - navei_x*navei_y;
                    double nvarj_xy = ((var_xy[j] + ave_x[j]*ave_y[j]) * p.G[j] - diff_xy) / p.G[j] - navej_x*navej_y;
    
                    double nsum = nvari_x + nvari_y + abs(nvari_xy)
                                + nvarj_x + nvarj_y + abs(nvarj_xy);
                    double diff = nsum - psum;
                    if(diff<0){
                        belong[u] = j;
                        belong[v] = i;
    
                        ave_x[i] = navei_x;
                        ave_x[j] = navej_x;
                        ave_y[i] = navei_y;
                        ave_y[j] = navej_y;
                        var_x[i] = nvari_x;
                        var_x[j] = nvarj_x;
                        var_y[i] = nvari_y;
                        var_y[j] = nvarj_y;
                        var_xy[i] = nvari_xy;
                        var_xy[j] = nvarj_xy;
    
                        groups[i].erase(u);
                        groups[j].erase(v);
                        groups[i].insert(v);
                        groups[j].insert(u);
                    }
                }();
            }
        }

        // 最小全域木を求める
        double score = 0;
        rep(i,0,p.M){
            score += struct_MST(groups[i],trees[i]);
        }

        // 占い
        vector<int> olacle_point(p.M,0);
        vector<int> idx(p.M);
        iota(all(idx),0);
        sort(all(idx),[&](int i, int j){ return p.G[i]>p.G[j]; });
        {
            int i = 0, q = 0;
            bool ok = true;
            while(ok && q<Q){
                ok = false;
                while(i<p.M && q<Q && p.G[idx[i]]-p.L*olacle_point[idx[i]]>=3){
                    olacle_point[idx[i]]++;
                    q++;
                    i++;
                    ok = true;
                }
                i = 0;
            }
        }
        for(int i : idx){
            [&](set<int> &group, vector<pair<int,int>> &tree, int op){
                int siz = group.size();
                vector<int> city(siz);
                {
                    int j = 0;
                    for(int c : group){
                        city[j] = c;
                        j++;
                    }
                }
                map<int,vector<int>> g;
                for(auto [a,b] : tree){
                    g[a].push_back(b);
                    g[b].push_back(a);
                }
                set<int> used;
                vector<vector<pair<int,int>>> query_tree;
                while(op>0){
                    op--;
                    // ランダムな1点からbfsにより最大L点の連結な頂点の組を取り出す
                    queue<int> que;
                    vector<int> query_group;
                    int s;
                    do{
                        s = mt()%siz;
                    }while(used.count(city[s]));
                    que.push(city[s]);
                    query_group.push_back(city[s]);
                    used.insert(city[s]);

                    int cnt = 1;
                    while(!que.empty()){
                        if(cnt>=p.L) break;
                        int u = que.front();
                        que.pop();
                        for(int v : g[u]){
                            if(cnt>=p.L) break;
                            if(used.count(v)) continue;
                            used.insert(v);
                            que.push(v);
                            query_group.push_back(v);
                            cnt++;
                        }
                    }
                    if(query_group.size()<3) continue;
                    query_tree.push_back(query(query_group));
                }
                if(query_tree.size()==0) return;
                reconst_MST(group,tree,query_tree);
            }(groups[i], trees[i], olacle_point[i]);
        }

        print_ans(groups,trees);
    }
};

int main()
{
    Solver sol;
    sol.solve();
}