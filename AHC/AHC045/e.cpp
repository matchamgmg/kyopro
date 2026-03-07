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
constexpr const int GridNum = 25;
constexpr const int GridSize = 400;

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

    void estimate(vector<vector<pair<int,int>>> &query_trees, int a, int siz){
        auto check1 = [&](int i, int j, int x, int y) -> bool {// Aに隣接する頂点(登場順i->j) or edgeにおいてiがAに隣接する
            int x1 = center[i].x, x2 = center[j].x, y1 = center[i].y, y2 = center[j].y;
            return -2*x*(x1-x2) -2*y*(y1-y2) +(x1*x1-x2*x2) +(y1*y1-y2*y2) <= 0;
        };
        auto check2 = [&](int i, int j, int x, int y) -> bool {// edgeがAを含まず、iがAと連結でない
            int xi = center[i].x, xj = center[j].x, yi = center[i].y, yj = center[j].y;
            return (x*x-xj*xj) + (y*y-yj*yj) - 2*xi*(x-xj) - 2*yi*(y-yj) >= 0;
        };

        vector<pair<int,int>> condi1, condi2;

        rep(i,0,siz){
            map<int,int> dict;
            int num = 0;
            for(auto [u,v] : query_trees[i]){
                if(!dict.count(u)){
                    dict[u] = num;
                    num++;
                }
                if(!dict.count(v)){
                    dict[v] = num;
                    num++;
                }
            }
            set<int> next2A_set;
            vector<int> next2A_vec;
            dsu uf(num);
            for(auto [u,v] : query_trees[i]){
                if(u==a){
                    next2A_set.insert(v);
                    next2A_vec.push_back(v);
                }else if(v==a){
                    next2A_set.insert(u);
                    next2A_vec.push_back(u);
                }else{
                    if(!uf.same(dict[a],dict[u])){
                        condi2.push_back({u,v});
                    }
                    if(!uf.same(dict[a],dict[v])){
                        condi2.push_back({v,u});
                    }
                    if(next2A_set.count(u)){
                        condi1.push_back({u,v});
                    }else if(next2A_set.count(v)){
                        condi1.push_back({v,u});
                    }
                }
                uf.merge(dict[u],dict[v]);
            }
            rep(j,0,next2A_vec.size()-1){
                rep(k,j+1,next2A_vec.size()){
                    condi1.push_back({next2A_vec[j],next2A_vec[k]});
                }
            }
        }

        //aの座標をランダムに試して，条件を満たすものの平均をとる

        ll nx = 0, ny = 0;
        int cnt = 0;
        int x = p.R[a].lx, y = p.R[a].ly;
        int wx = p.R[a].rx-p.R[a].lx, wy = p.R[a].ry-p.R[a].ly;
        int conditions = condi1.size() + condi2.size();
        int64_t now_time = tk.get_time();
        while(tk.get_time()-now_time<=1){
            int xx = x + mt()%(wx+1);
            int yy = y + mt()%(wy+1);
            int ok = 0;
            for(auto [a,b] : condi1){
                ok += check1(a,b,xx,yy);
            }
            for(auto [a,b] : condi2){
                ok += check2(a,b,xx,yy);
            }
            if((double)ok/conditions>=0.6){
                nx += xx;
                ny += yy;
                cnt++;
            }
        }
        if(cnt==0) return;
        nx /= cnt;
        ny /= cnt;
        if(nx>10000) nx = 10000;
        if(ny>10000) ny = 10000;
        center[a].x = nx;
        center[a].y = ny;
        return;
    }

    void solve(){
        // 占いの回数を割り振る
        vector<int> op1_vec(p.M);
        int op2 = Q;
        {
            rep(i,0,p.M){
                op1_vec[i] = (p.G[i]-3>=0 ? (p.G[i]-3)/p.L + 1 : 0);
                op2 -= op1_vec[i];
            }
        }

        // 占いによって都市の位置を推定する
        {
            // 占い回数割り当て
            array<int,N> idx;
            array<int,N> wi;
            array<int,N> op2_vec;
            rep(i,0,N){
                wi[i] = max(p.R[i].rx-p.R[i].lx,p.R[i].ry-p.R[i].ly);
            }
            iota(all(idx),0);
            sort(all(idx),[&](int i, int j){ return wi[i]<wi[j]; });

            {
                int op;
                if(p.L>=10) op = 1;
                else if(p.L>=7) op = 2;
                else if(p.L>=4) op = 3;
                else op = 4;
    
                rep(i,0,N){
                    op2_vec[idx[N-i-1]] = min(op2,op);
                    op2 -= op2_vec[idx[N-i-1]];
                }
            }

            // 占い
            int b_max = 100;

            rep(i,0,N){
                int op = op2_vec[idx[N-i-1]];
                if(op==0) continue;
                vector<vector<int>> query_vecs(op);
                vector<vector<pair<int,int>>> query_trees(op);
                int c = 0;
                while(c<op){
                    set<int> used;
                    query_vecs[c].push_back(idx[N-i-1]);
                    used.insert(idx[N-i-1]);
                    rep(j,0,p.L-1){
                        int b;
                        do{
                            b = mt()%b_max;
                        }while(used.count(idx[b]));
                        query_vecs[c].push_back(idx[b]);
                        used.insert(idx[b]);
                    }
                    query_trees[c] = query(query_vecs[c]);
                    c++;
                }
                estimate(query_trees, idx[N-i-1],op);
            }
        }

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


        array<double,M_max> ave_x, ave_y, var_x, var_y;
        rep(i,0,p.M){
            ave_x[i] = 0;
            ave_y[i] = 0;
            var_x[i] = 0;
            var_y[i] = 0;
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
            }
            // 各グループの平均・分散・共分散を求める
            ave_x[i] /= cnt;
            ave_y[i] /= cnt;
            var_x[i] = var_x[i]/cnt - ave_x[i]*ave_x[i];
            var_y[i] = var_y[i]/cnt - ave_y[i]*ave_y[i];
        }

        // 山登り
        if(p.M>1){
            int limit = max(500,500+(p.L-5)*50);
            ll now_time = tk.get_time();
            while(tk.get_time()-now_time<=limit){
                [&](){
                    int u,v;
                    u = mt()%N;
                    do{
                        v = mt()%N;
                    }while(belong[v]==belong[u]);
    
                    // 差分更新
                    int i = belong[u], j = belong[v];
                    double psum = var_x[i] + var_y[i]
                                + var_x[j] + var_y[j];
                    
                    int diff_x = center[v].x - center[u].x;
                    int diff_y = center[v].y - center[u].y;
                    int diff_xx = center[v].x*center[v].x - center[u].x*center[u].x;
                    int diff_yy = center[v].y*center[v].y - center[u].y*center[u].y;
    
                    double navei_x = (ave_x[i] * p.G[i] + diff_x) / p.G[i];
                    double navej_x = (ave_x[j] * p.G[j] - diff_x) / p.G[j];
                    double navei_y = (ave_y[i] * p.G[i] + diff_y) / p.G[i];
                    double navej_y = (ave_y[j] * p.G[j] - diff_y) / p.G[j];
    
                    double nvari_x = ((var_x[i] + ave_x[i]*ave_x[i]) * p.G[i] + diff_xx) / p.G[i] - navei_x*navei_x;
                    double nvarj_x = ((var_x[j] + ave_x[j]*ave_x[j]) * p.G[j] - diff_xx) / p.G[j] - navej_x*navej_x;
                    double nvari_y = ((var_y[i] + ave_y[i]*ave_y[i]) * p.G[i] + diff_yy) / p.G[i] - navei_y*navei_y;
                    double nvarj_y = ((var_y[j] + ave_y[j]*ave_y[j]) * p.G[j] - diff_yy) / p.G[j] - navej_y*navej_y;
    
                    double nsum = nvari_x + nvari_y
                                + nvarj_x + nvarj_y;
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
        rep(i,0,p.M){
            int op = op1_vec[i];
            if(op==0) continue;
            map<int,vector<int>> g;
            {
                for(auto [a,b] : trees[i]){
                    g[a].push_back(b);
                    g[b].push_back(a);
                }
            }
            int siz_group = groups[i].size();
            vector<int> dict(siz_group);
            {
                int num = 0;
                for(auto t : groups[i]){
                    dict[num] = t;
                    num++;
                }
            }
            vector<vector<int>> best_query_vecs(op);
            {
                int best_cnt = -1;
                ll now_time = tk.get_time();
                while(tk.get_time()-now_time<=op){
                    vector<vector<int>> query_vecs(op);
                    set<int> used;
                    queue<pair<int,int>> q_tag;
                    {
                        int a;
                        rep(c,0,op){
                            do{
                                a = mt()%siz_group;
                            }while(used.count(dict[a]));
                            used.insert(dict[a]);
                            q_tag.push({dict[a],c});
                            query_vecs[c].push_back(dict[a]);
                        }
                    }
                    // bfs
                    while(!q_tag.empty()){
                        auto [u,t] = q_tag.front();
                        q_tag.pop();
                        if(query_vecs[t].size()>=p.L) continue;
                        for(int v : g[u]){
                            if(query_vecs[t].size()>=p.L) break;
                            if(used.count(v)) continue;
                            used.insert(v);
                            q_tag.push({v,t});
                            query_vecs[t].push_back(v);
                        }
                    }
                    int cnt = 0;
                    // count
                    rep(c,0,op){
                        cnt += (query_vecs[c].size()>2 ? query_vecs[c].size() : 0);
                    }
                    if(cnt > best_cnt){
                        best_cnt = cnt;
                        best_query_vecs = query_vecs;
                    }
                }
            }
            vector<vector<pair<int,int>>> query_trees;
            {
                rep(c,0,op){
                    if(best_query_vecs[c].size()<=2) continue;
                    query_trees.push_back(query(best_query_vecs[c]));
                }
            }
            reconst_MST(groups[i],trees[i],query_trees);
        }


        print_ans(groups,trees);
    }
};

int main()
{
    Solver sol;
    sol.solve();
}