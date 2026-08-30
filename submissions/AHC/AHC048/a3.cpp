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

constexpr const int N = 20;
constexpr const int H = 1000;

struct CMY {
    array<double,3> col;
    CMY(){
        col = {0,0,0};
    }
};

struct ColorManage {
    CMY col;
    int remain;
    pair<int,int> wel_num;
    ColorManage(){
        remain = 0;
        wel_num = {-1,-1};
        col.col = {-1,-1,-1};
    }
};

struct Problem {
    int n,h;
    int K,T,D;
    array<CMY,20> own;
    array<CMY,H> target;

    void load(){
        cin >> n >> K >> h >> T >> D;
        rep(i,0,K){
            rep(j,0,3){
                cin >> own[i].col[j];
            }
        }
        rep(i,0,H){
            rep(j,0,3){
                cin >> target[i].col[j];
            }
        }
    }
};

struct Answer {
    array<array<bool,N-1>,N> v;
    array<array<bool,N>,N-1> h;
    vector<int> ans_num;
    vector<vector<int>> ans_query;

    ll score;
    double E;
    int V;
    Answer(){
        score = 1LL<<60;
        E = 0;
        V = 0;
    }

    void print_ans(){
        rep(i,0,N){
            rep(j,0,N-1){
                cout << v[i][j] << " \n"[j==N-2];
            }
        }
        rep(i,0,N-1){
            rep(j,0,N){
                cout << h[i][j] << " \n"[j==N-1];
            }
        }
        rep(i,0,ans_num.size()){
            cout << ans_num[i] << ' ';
            rep(j,0,ans_query[i].size()){
                cout << ans_query[i][j] << " \n"[j==(ans_query[i].size()-1)];
            }
        }
    }

    void fprint_ans(){
        ofstream ofs("0000.txt");
        rep(i,0,N){
            rep(j,0,N-1){
                ofs << v[i][j] << " \n"[j==N-2];
            }
        }
        rep(i,0,N-1){
            rep(j,0,N){
                ofs << h[i][j] << " \n"[j==N-1];
            }
        }
        rep(i,0,ans_num.size()){
            ofs << ans_num[i] << ' ';
            rep(j,0,ans_query[i].size()){
                ofs << ans_query[i][j] << " \n"[j==(ans_query[i].size()-1)];
            }
        }
        ofs.close();
    }
};

struct Ink {
    array<double,3> col;
    int space; // 1masu, 2masu, 3masu
    int weight; // 1g, 2g, 3g
    int type; // space == 3のときだけ使う 
    bool add; // t: 継ぎ足し, f: 使い切り
    vector<int> recipe;
    array<double,3> sub_col;
};

struct Wel2 {
    int idx;
    pair<int,int> point;
    array<int,4> separator;
    int free_use; // 0: none, 1: 1g_free
    int remain;
    array<double,3> col;

    Wel2(){}

    Wel2(int i, int j){
        idx = j-1;
        point = {i,j};
        separator = {i,j,i+1,j};
        free_use = 0;
        remain = 0;
    }
};

struct Wel3 {
    int idx;
    pair<int,int> point;
    array<array<int,4>,2> separator;
    int free_use; // 0: none, 1: 1g_free, 2: 2g_free
    int remain;
    array<double,3> col;

    Wel3(){}

    Wel3(int i, int j){
        idx = i*20+j;
        point = {i,j};
        separator[0] = {i,j,i+1,j};
        separator[1] = {i+1,j,i+2,j};
        free_use = 0;
        remain = 0;
    }
};

struct reservation {
    int type; // 0: none, 2: wel2, 3: wel3
    int idx;

    reservation(){
        type = 0;
    }
};

bool operator<(const Ink &a, const Ink &b){
    return ((a.weight > b.weight) ? true : (a.weight < b.weight) ? false :
            (a.add > b.add) ? true : false);
}

struct Solver {
    Problem p;
    Answer best_ans;
    mt19937 mt;
    Solver(){
        p.load();
        mt = mt19937(0);
    }

    void calc_score(Answer &ans_){
        ans_.score =  1 + p.D*(ans_.V - H) + (ll)round(10000 * ans_.E);
    }

    void query1(Answer &ans, int i, int j, int k){
        ans.ans_num.push_back(1);
        ans.ans_query.push_back({i,j,k});
        ans.V++;
    }

    void query2(Answer &ans, int i, int j, double sc){
        ans.ans_num.push_back(2);
        ans.ans_query.push_back({i,j});
        ans.E += sc;
    }

    void query3(Answer &ans, int i, int j){
        ans.ans_num.push_back(3);
        ans.ans_query.push_back({i,j});
    }

    void query4(Answer &ans, int i1, int j1, int i2, int j2){
        ans.ans_num.push_back(4);
        ans.ans_query.push_back({i1,j1,i2,j2});
    }

    void struct_separator_4_4(Answer &ans){
        rep(i,0,N){
            rep(j,0,N-1){
                ans.v[i][j] = (j%4==3);
            }
        }
        rep(i,0,N-1){
            rep(j,0,N){
                ans.h[i][j] = (i%4==3);
            }
        }
    }

    void struct_separator_2_1(Answer &ans){
        rep(i,0,N){
            rep(j,0,N-1){
                ans.v[i][j] = 1;
            }
        }
        rep(i,0,N-1){
            rep(j,0,N){
                ans.h[i][j] = (i%2==1);
            }
        }
    }

    void struct_separator_1_1_2_1_3_1(Answer &ans){
        rep(i,0,N){
            rep(j,0,N-1){
                ans.v[i][j] = 1;
            }
        }
        rep(i,0,N-1){
            rep(j,0,N){
                ans.h[i][j] = (i%3==1 || (i==0 && j==0));
            }
        }
    }

    double calc_e(CMY &col, CMY &tar){
        if(col.col[0]<0 && col.col[1]<0 && col.col[2]<0) return 100;
        double res = 0;
        rep(i,0,3){
            res += (col.col[i]-tar.col[i]) * (col.col[i]-tar.col[i]);
        }
        return sqrtf64(res);
    }

    double calc_e_1_1_mix(int a, int b, CMY &tar){
        CMY col;
        rep(i,0,3){
            col.col[i] = (p.own[a].col[i] + p.own[b].col[i]) / 2;
        }
        double res = 0;
        rep(i,0,3){
            res += (col.col[i]-tar.col[i]) * (col.col[i]-tar.col[i]);
        }
        return sqrtf64(res);
    }

    void solve_no_mix(){
        Answer ans;
        struct_separator_4_4(ans);
        rep(i,0,H){
            int k = -1;
            double best_score = 10;
            rep(j,0,p.K){
                double score = calc_e(p.own[j],p.target[i]);
                if(best_score > score){
                    k = j;
                    best_score = score;
                }
            }
            ans.ans_num.push_back(1);
            ans.ans_query.push_back({0,0,k});
            ans.V++;
            ans.ans_num.push_back(2);
            ans.ans_query.push_back({0,0});
            ans.E += best_score;
        }
        calc_score(ans);
        if(best_ans.score > ans.score){
            best_ans = ans;
        }
    }

    void solve_1_1_mix(){
        Answer ans;
        struct_separator_2_1(ans);
        vector<vector<ColorManage>> pallet(p.K,vector<ColorManage>(p.K));
        vector<vector<pair<int,int>>> code_map(N,vector<pair<int,int>>(N,{-1,-1}));
        rep(i,0,p.K){
            rep(j,0,p.K){
                pallet[i][j].remain = 0;
                pallet[i][j].wel_num = {-1,-1};
            }
        }
        stack<pair<int,int>> open_wel;
        for(int i=0; i<N; i+=2){
            rep(j,0,N){
                open_wel.push({i,j});
            }
        }
        rep(c,0,H){
            double best_score = 100;
            pair<int,int> best_code = {-1,-1};
            rep(i,0,p.K){
                rep(j,i,p.K){
                    double score = calc_e_1_1_mix(i,j,p.target[c]);
                    if(best_score > score){
                        best_score = score;
                        best_code = {i,j};
                    }
                }
            }
            // remain > 0 ?
            ColorManage &cm = pallet[best_code.first][best_code.second];
            if(cm.remain > 0){
                query2(ans,cm.wel_num.first,cm.wel_num.second,best_score);
                cm.remain--;
                if(cm.remain<=0){
                    open_wel.push({cm.wel_num.first,cm.wel_num.second});
                }
            }else{
                if(open_wel.empty()){
                    int ri = (mt()%(N/2))*2;
                    int rj = mt()%N;
                    query3(ans,ri,rj);
                    open_wel.push({ri,rj});
                    auto code = code_map[ri][rj];
                    ColorManage &rcm = pallet[code.first][code.second];
                    rcm.remain = 0;
                }
                auto wel = open_wel.top();
                open_wel.pop();
                query1(ans,wel.first,wel.second,best_code.first);
                if(best_code.first!=best_code.second){
                    query1(ans,wel.first,wel.second,best_code.second);

                }
                query2(ans,wel.first,wel.second,best_score);
                if(best_code.first==best_code.second){
                    open_wel.push(wel);
                }else{
                    cm.remain = 1;
                    cm.wel_num = wel;
                    code_map[wel.first][wel.second] = best_code;
                }
            }
        }
        calc_score(ans);
        if(best_ans.score > ans.score){
            best_ans = ans;
        }
    }

    void solve_1_1_mix_1_1_1_mix(){
        Answer ans;
        // separator
        struct_separator_1_1_2_1_3_1(ans);
        // pallet
        vector pallet(p.K+1,vector<vector<ColorManage>>(p.K+1,vector<ColorManage>(p.K+1)));
        vector wel2code(N,vector<vector<int>>(N,vector<int>(3,-1)));
        auto f = [&](int i, int j, int k) -> bool {
            bool res = false;
            // res |= (i<p.K && j==p.K && k==p.K);
            // res |= (i!=j && i<p.K && j<p.K && k==p.K);
            // res |= (i==j && i!=k && i<p.K && k<p.K);
            // res |= (i!=j && j!=k && k!=i && i<p.K && j<p.K && k<p.K);
            if(i<p.K){
                if(j<p.K){
                    if(k<p.K){
                        if(i!=k && j!=k) res = true;
                    }else{
                        if(i!=j) res = true;
                    }
                }else{
                    if(k==p.K){
                        res = true;
                    }
                }
            }
            if(i!=j && j!=k && k!=i){
                if(!(i<j && j<k)) res = false;
            }
            return res;
        };
        rep(i,0,p.K+1){
            rep(j,0,p.K+1){
                rep(k,0,p.K+1){
                    if(!f(i,j,k)) continue;
                    // mix
                    int v = 0;
                    v += (i!=p.K) + (j!=p.K) + (k!=p.K);
                    CMY col;
                    if(i!=p.K){
                        rep(c,0,3){
                            col.col[c] += p.own[i].col[c];
                        }
                    }
                    if(j!=p.K){
                        rep(c,0,3){
                            col.col[c] += p.own[j].col[c];
                        }
                    }
                    if(k!=p.K){
                        rep(c,0,3){
                            col.col[c] += p.own[k].col[c];
                        }
                    }
                    if(v!=0){
                        rep(c,0,3){
                            col.col[c] /= v;
                        }
                        pallet[i][j][k].col = col;
                    }
                }
            }
        }
        // open_wel
        stack<pair<int,int>> open_wel_1, open_wel_2, open_wel_3;
        rep(i,0,2){
            open_wel_1.push({i,0});
        }
        rep(i,1,N){
            open_wel_2.push({0,i});
        }
        rep(i,0,6){
            rep(j,0,N){
                open_wel_3.push({3*i+2,j});
            }
        }
        // simulate
        rep(c,0,H){
            // best
            vector<int> best_code(3,-1);
            double best_score = 10;
            rep(i,0,p.K+1){
                rep(j,0,p.K+1){
                    rep(k,0,p.K+1){
                        double score = calc_e(pallet[i][j][k].col,p.target[c]);
                        if(best_score > score){
                            best_score = score;
                            best_code = {i,j,k};
                        }
                    }
                }
            }
            // query
            int col_dim = (best_code[0]!=p.K) + (best_code[1]!=p.K) + (best_code[2]!=p.K);
            ColorManage &cm = pallet[best_code[0]][best_code[1]][best_code[2]];
            if(cm.remain > 0){
                query2(ans,cm.wel_num.first,cm.wel_num.second,best_score);
                cm.remain--;
                if(cm.remain <= 0){
                    (col_dim==1 ? open_wel_1 : col_dim==2 ? open_wel_2 : open_wel_3).push(cm.wel_num);
                }
            }else{
                if(col_dim==1){
                    if(open_wel_1.empty()){
                        int ri = mt()%2;
                        auto rcode = wel2code[ri][0];
                        ColorManage &rcm = pallet[rcode[0]][rcode[1]][rcode[2]];
                        rep(i,0,rcm.remain){
                            query3(ans,rcm.wel_num.first,rcm.wel_num.second);
                        }
                        rcm.remain = 0;
                        open_wel_1.push(rcm.wel_num);
                    }
                    auto wel_address = open_wel_1.top();
                    open_wel_1.pop();
                    query1(ans,wel_address.first,wel_address.second,best_code[0]);
                    query2(ans,wel_address.first,wel_address.second,best_score);
                    open_wel_1.push(wel_address);
                }else if(col_dim==2){
                    if(open_wel_2.empty()){
                        int rj;
                        int vv;
                        do{
                            rj = mt()%N;
                            auto v = wel2code[0][rj];
                            vv = (v[0]<0) + (v[1]<0) + (v[2]<0);
                        }while(rj==0 || vv==3);
                        auto rcode = wel2code[0][rj];
                        ColorManage &rcm = pallet[rcode[0]][rcode[1]][rcode[2]];
                        rep(i,0,rcm.remain){
                            query3(ans,rcm.wel_num.first,rcm.wel_num.second);
                        }
                        rcm.remain = 0;
                        open_wel_2.push(rcm.wel_num);
                    }
                    auto wel_address = open_wel_2.top();
                    open_wel_2.pop();
                    query1(ans,wel_address.first,wel_address.second,best_code[0]);
                    query1(ans,wel_address.first,wel_address.second,best_code[1]);
                    query2(ans,wel_address.first,wel_address.second,best_score);
                    cm.remain = 1;
                    cm.wel_num = wel_address;
                    wel2code[wel_address.first][wel_address.second] = best_code;
                }else{
                    if(open_wel_3.empty()){
                        int ri,rj;
                        int vv;
                        do{
                            ri = mt()%N;
                            rj = mt()%N;
                            auto v = wel2code[ri][rj];
                            vv = (v[0]<0) + (v[1]<0) + (v[2]<0);
                        }while(ri<=1 || vv==3);
                        auto rcode = wel2code[ri][rj];
                        ColorManage &rcm = pallet[rcode[0]][rcode[1]][rcode[2]];
                        rep(i,0,rcm.remain){
                            query3(ans,rcm.wel_num.first,rcm.wel_num.second);
                        }
                        rcm.remain = 0;
                        open_wel_3.push(rcm.wel_num);
                    }
                    auto wel_address = open_wel_3.top();
                    open_wel_3.pop();
                    query1(ans,wel_address.first,wel_address.second,best_code[0]);
                    query1(ans,wel_address.first,wel_address.second,best_code[1]);
                    query1(ans,wel_address.first,wel_address.second,best_code[2]);
                    query2(ans,wel_address.first,wel_address.second,best_score);
                    cm.remain = 2;
                    cm.wel_num = wel_address;
                    wel2code[wel_address.first][wel_address.second] = best_code;
                }
            }
        }
        calc_score(ans);
        if(best_ans.score > ans.score){
            best_ans = ans;
        }
    }

    void solve_1_1_mix_1_1_1_mix_reuse(){
        Answer ans;
        // separator
        struct_separator_1_1_2_1_3_1(ans);
        // pallet
        vector pallet(p.K+1,vector<vector<ColorManage>>(p.K+1,vector<ColorManage>(p.K+1)));
        vector wel2code(N,vector<vector<int>>(N,vector<int>(3,-1)));
        vector used(N,vector<bool>(N,false));
        auto f = [&](int i, int j, int k) -> bool {
            bool res = false;
            // res |= (i<p.K && j==p.K && k==p.K);
            // res |= (i!=j && i<p.K && j<p.K && k==p.K);
            // res |= (i==j && i!=k && i<p.K && k<p.K);
            // res |= (i!=j && j!=k && k!=i && i<p.K && j<p.K && k<p.K);
            if(i<p.K){
                if(j<p.K){
                    if(k<p.K){
                        if(i!=k && j!=k) res = true;
                    }else{
                        if(i!=j) res = true;
                    }
                }else{
                    if(k==p.K){
                        res = true;
                    }
                }
            }
            if(i!=j && j!=k && k!=i){
                if(!(i<j && j<k)) res = false;
            }
            return res;
        };
        rep(i,0,p.K+1){
            rep(j,0,p.K+1){
                rep(k,0,p.K+1){
                    if(!f(i,j,k)) continue;
                    // mix
                    int v = 0;
                    v += (i!=p.K) + (j!=p.K) + (k!=p.K);
                    CMY col;
                    if(i!=p.K){
                        rep(c,0,3){
                            col.col[c] += p.own[i].col[c];
                        }
                    }
                    if(j!=p.K){
                        rep(c,0,3){
                            col.col[c] += p.own[j].col[c];
                        }
                    }
                    if(k!=p.K){
                        rep(c,0,3){
                            col.col[c] += p.own[k].col[c];
                        }
                    }
                    if(v!=0){
                        rep(c,0,3){
                            col.col[c] /= v;
                        }
                        pallet[i][j][k].col = col;
                    }
                }
            }
        }
        // open_wel
        stack<pair<int,int>> open_wel_1, open_wel_2, open_wel_3;
        rep(i,0,2){
            open_wel_1.push({i,0});
        }
        rep(i,1,N){
            open_wel_2.push({0,i});
        }
        rep(i,0,6){
            rep(j,0,N){
                open_wel_3.push({3*i+2,j});
            }
        }
        // simulate
        rep(c,0,H){
            // best
            vector<int> best_code(3,-1);
            double best_score = 10;
            rep(i,0,p.K+1){
                rep(j,0,p.K+1){
                    rep(k,0,p.K+1){
                        double score = calc_e(pallet[i][j][k].col,p.target[c]);
                        if(best_score > score){
                            best_score = score;
                            best_code = {i,j,k};
                        }
                    }
                }
            }
            // reuse
            ColorManage &cm = pallet[best_code[0]][best_code[1]][best_code[2]];
            vector<int> best_alt_v;
            double best_alt_score = 1000;
            auto g = [&](double a) -> bool {
                return (a<best_score*1.02);
            };
            rep(i,0,N){
                rep(j,0,N){
                    if(!used[i][j]) continue;
                    auto v = wel2code[i][j];
                    ColorManage &rcm = pallet[v[0]][v[1]][v[2]];
                    double alt_score = calc_e(rcm.col,p.target[c]);
                    if(best_alt_score > alt_score){
                        best_alt_score = alt_score;
                        best_alt_v = v;
                    }
                }
            }
            if(g(best_alt_score)){
                ColorManage &best_alt = pallet[best_alt_v[0]][best_alt_v[1]][best_alt_v[2]];
                query2(ans,best_alt.wel_num.first,best_alt.wel_num.second,best_alt_score);
                best_alt.remain--;
                if(best_alt.remain <= 0){
                    int col_dim = (best_alt_v[0]!=p.K) + (best_alt_v[1]!=p.K) + (best_alt_v[2]!=p.K);
                    (col_dim==1 ? open_wel_1 : col_dim==2 ? open_wel_2 : open_wel_3).push(best_alt.wel_num);
                    used[best_alt.wel_num.first][best_alt.wel_num.second] = false;
                }
                continue;
            }
            // query
            int col_dim = (best_code[0]!=p.K) + (best_code[1]!=p.K) + (best_code[2]!=p.K);
            if(cm.remain > 0){
                query2(ans,cm.wel_num.first,cm.wel_num.second,best_score);
                cm.remain--;
                if(cm.remain <= 0){
                    (col_dim==1 ? open_wel_1 : col_dim==2 ? open_wel_2 : open_wel_3).push(cm.wel_num);
                    used[cm.wel_num.first][cm.wel_num.second] = false;
                }
            }else{
                if(col_dim==1){
                    if(open_wel_1.empty()){
                        int ri = mt()%2;
                        auto rcode = wel2code[ri][0];
                        ColorManage &rcm = pallet[rcode[0]][rcode[1]][rcode[2]];
                        rep(i,0,rcm.remain){
                            query3(ans,rcm.wel_num.first,rcm.wel_num.second);
                        }
                        rcm.remain = 0;
                        open_wel_1.push(rcm.wel_num);
                    }
                    auto wel_address = open_wel_1.top();
                    open_wel_1.pop();
                    query1(ans,wel_address.first,wel_address.second,best_code[0]);
                    query2(ans,wel_address.first,wel_address.second,best_score);
                    open_wel_1.push(wel_address);
                }else if(col_dim==2){
                    if(open_wel_2.empty()){
                        int rj;
                        int vv;
                        do{
                            rj = mt()%N;
                            auto v = wel2code[0][rj];
                            vv = (v[0]<0) + (v[1]<0) + (v[2]<0);
                        }while(rj==0 || vv==3);
                        auto rcode = wel2code[0][rj];
                        ColorManage &rcm = pallet[rcode[0]][rcode[1]][rcode[2]];
                        rep(i,0,rcm.remain){
                            query3(ans,rcm.wel_num.first,rcm.wel_num.second);
                        }
                        rcm.remain = 0;
                        open_wel_2.push(rcm.wel_num);
                    }
                    auto wel_address = open_wel_2.top();
                    open_wel_2.pop();
                    query1(ans,wel_address.first,wel_address.second,best_code[0]);
                    query1(ans,wel_address.first,wel_address.second,best_code[1]);
                    query2(ans,wel_address.first,wel_address.second,best_score);
                    cm.remain = 1;
                    cm.wel_num = wel_address;
                    wel2code[wel_address.first][wel_address.second] = best_code;
                    used[wel_address.first][wel_address.second] = true;
                }else{
                    if(open_wel_3.empty()){
                        int ri,rj;
                        int vv;
                        do{
                            ri = mt()%N;
                            rj = mt()%N;
                            auto v = wel2code[ri][rj];
                            vv = (v[0]<0) + (v[1]<0) + (v[2]<0);
                        }while(ri<=1 || vv==3);
                        auto rcode = wel2code[ri][rj];
                        ColorManage &rcm = pallet[rcode[0]][rcode[1]][rcode[2]];
                        rep(i,0,rcm.remain){
                            query3(ans,rcm.wel_num.first,rcm.wel_num.second);
                        }
                        rcm.remain = 0;
                        open_wel_3.push(rcm.wel_num);
                    }
                    auto wel_address = open_wel_3.top();
                    open_wel_3.pop();
                    query1(ans,wel_address.first,wel_address.second,best_code[0]);
                    query1(ans,wel_address.first,wel_address.second,best_code[1]);
                    query1(ans,wel_address.first,wel_address.second,best_code[2]);
                    query2(ans,wel_address.first,wel_address.second,best_score);
                    cm.remain = 2;
                    cm.wel_num = wel_address;
                    wel2code[wel_address.first][wel_address.second] = best_code;
                    used[wel_address.first][wel_address.second] = true;
                }
            }
        }
        calc_score(ans);
        if(best_ans.score > ans.score){
            best_ans = ans;
        }
    }

    // mix functions
    // 絵具は配列でもつ
    void solve_froward(){
        int t = 0;
        Answer ans;
        struct_separator_1_1_2_1_3_1(ans);
        vector<Ink> ink_list;
        // make_ink_list
        [&]() -> void {
            // 1g
            rep(i,0,p.K){
                Ink ink;
                ink.weight = 1;
                ink.space = 1;
                ink.add = false;
                rep(c,0,3){
                    ink.col[c] = p.own[i].col[c];
                }
                ink.recipe.push_back(i);

                ink_list.push_back(ink);
            }
            // 2g
            rep(i,0,p.K-1){
                rep(j,i+1,p.K){
                    // 1:1
                    {
                        Ink ink;
                        ink.weight = 2;
                        ink.add = false;
                        ink.space = 2;
                        rep(c,0,3){
                            ink.col[c] = (p.own[i].col[c] + p.own[j].col[c]);
                            ink.col[c] /= 2;
                            ink.sub_col[c] = ink.col[c];
                        }
                        ink.recipe.push_back(i);
                        ink.recipe.push_back(j);

                        ink_list.push_back(ink);
                        ink.add = true;
                        ink_list.push_back(ink);
                    }
                    // 2:1
                    {
                        Ink ink;
                        ink.weight = 2;
                        ink.add = false;
                        ink.space = 3;
                        ink.type = 0;
                        rep(c,0,3){
                            ink.col[c] = (2*p.own[i].col[c] + p.own[j].col[c]);
                            ink.col[c] /= 3;
                            ink.sub_col[c] = (2*p.own[j].col[c] + p.own[i].col[c]);
                            ink.sub_col[c] /= 3;
                        }
                        ink.recipe.push_back(j);
                        ink.recipe.push_back(i);

                        ink_list.push_back(ink);
                        ink.add = true;
                        ink_list.push_back(ink);
                    }
                    // 1:2
                    {
                        Ink ink;
                        ink.weight = 2;
                        ink.add = false;
                        ink.space = 3;
                        ink.type = 0;
                        rep(c,0,3){
                            ink.col[c] = (p.own[i].col[c] + 2*p.own[j].col[c]);
                            ink.col[c] /= 3;
                            ink.sub_col[c] = (p.own[j].col[c] + 2*p.own[i].col[c]);
                            ink.sub_col[c] /= 3;
                        }
                        ink.recipe.push_back(j);
                        ink.recipe.push_back(i);

                        ink_list.push_back(ink);
                        ink.add = true;
                        ink_list.push_back(ink);
                    }
                    // 3:2
                    {
                        Ink ink;
                        ink.weight = 2;
                        ink.add = false;
                        ink.space = 3;
                        ink.type = 1;
                        rep(c,0,3){
                            ink.col[c] = (3*p.own[i].col[c] + 2*p.own[j].col[c]);
                            ink.col[c] /= 5;
                            ink.sub_col[c] = (3*p.own[j].col[c] + 2*p.own[i].col[c]);
                            ink.sub_col[c] /= 5;
                        }
                        ink.recipe.push_back(j);
                        ink.recipe.push_back(i);

                        ink_list.push_back(ink);
                        ink.add = true;
                        ink_list.push_back(ink);
                    }
                    // 2:3
                    {
                        Ink ink;
                        ink.weight = 2;
                        ink.add = false;
                        ink.space = 3;
                        ink.type = 1;
                        rep(c,0,3){
                            ink.col[c] = (2*p.own[i].col[c] + 3*p.own[j].col[c]);
                            ink.col[c] /= 5;
                            ink.sub_col[c] = (2*p.own[j].col[c] + 3*p.own[i].col[c]);
                            ink.sub_col[c] /= 5;
                        }
                        ink.recipe.push_back(j);
                        ink.recipe.push_back(i);

                        ink_list.push_back(ink);
                        ink.add = true;
                        ink_list.push_back(ink);
                    }
                }
            }
            // 3g
            // 2color
            rep(i,0,p.K-1){
                rep(j,i+1,p.K){
                    // 2:1
                    {
                        Ink ink;
                        ink.weight = 3;
                        ink.add = false;
                        ink.space = 3;
                        ink.type = 2;
                        rep(c,0,3){
                            ink.col[c] = (2*p.own[i].col[c] + p.own[j].col[c]);
                            ink.col[c] /= 3;
                            ink.sub_col[c] = ink.col[c];
                        }
                        ink.recipe.push_back(i);
                        ink.recipe.push_back(i);
                        ink.recipe.push_back(j);

                        ink_list.push_back(ink);
                    }
                    // 1:2
                    {
                        Ink ink;
                        ink.weight = 3;
                        ink.add = false;
                        ink.space = 3;
                        ink.type = 2;
                        rep(c,0,3){
                            ink.col[c] = (p.own[i].col[c] + 2*p.own[j].col[c]);
                            ink.col[c] /= 3;
                            ink.sub_col[c] = ink.col[c];
                        }
                        ink.recipe.push_back(i);
                        ink.recipe.push_back(j);
                        ink.recipe.push_back(j);

                        ink_list.push_back(ink);
                    }
                }
            }
            // 3color
            rep(i,0,p.K-2){
                rep(j,i+1,p.K-1){
                    rep(k,j+1,p.K){
                        Ink ink;
                        ink.weight = 3;
                        ink.add = false;
                        ink.space = 3;
                        ink.type = 2;
                        rep(c,0,3){
                            ink.col[c] = (p.own[i].col[c] + p.own[j].col[c] + p.own[k].col[c]);
                            ink.col[c] /= 3;
                            ink.sub_col[c] = ink.col[c];
                        }
                        ink.recipe.push_back(i);
                        ink.recipe.push_back(j);
                        ink.recipe.push_back(k);

                        ink_list.push_back(ink);
                    }
                }
            }
        }();

        // targetに近い上位三色を求めておく
        vector<pq<pair<double,Ink>>> top3ink(H);
        auto calc_ee = [&](Ink &ink, CMY &tar) -> double {
            double res = 0;
            rep(c,0,3){
                res += (ink.col[c] - tar.col[c]) * (ink.col[c] - tar.col[c]);
            }
            res =  sqrtf64(res);
            return res;
        };
        [&]() -> void {
            rep(c,0,H){
                rep(i,0,ink_list.size()){
                    double e = calc_ee(ink_list[i],p.target[c]);
                    top3ink[c].push({e,ink_list[i]});
                    if(top3ink[c].size()>3) top3ink[c].pop();
                }
            }
        }();

        // 管理情報の初期化
        // 1x1 = (0,0)
        // 1x2 = Wel2vec,Wel2stack
        // 1x3 = Wel3vec,Wel3stack
        vector<Wel2> wel2vec(19);
        vector<Wel3> wel3vec(120);
        rep(i,1,20){
            wel2vec[i-1] = Wel2(0,i);
        }
        rep(i,0,6){
            rep(j,0,20){
                wel3vec[i*20+j] = Wel3(i*3+2,j);
            }
        }
        stack<Wel2> wel2stack;
        stack<Wel3> wel3stack;
        for(auto w : wel2vec){
            wel2stack.push(Wel2(w.point.first,w.point.second));
        }
        for(auto w : wel3vec){
            wel3stack.push(Wel3(w.point.first,w.point.second));
        }

        array<reservation,H> simu_reserve;

        // simulation
        auto use_ink = [&](Answer &ans, int wel_type, int wel_idx, int tar, bool free_use) -> void {
            if(wel_type==1){
                return;
            }else if(wel_type==2){
                Wel2 &wel = wel2vec[wel_idx];
                double sc = 0;
                rep(cc,0,3){
                    sc += (wel.col[cc] - p.target[tar].col[cc]) +(wel.col[cc] - p.target[tar].col[cc]);
                }
                sc = sqrtf64(sc);
                query2(ans,wel.point.first,wel.point.second,sc);
                t++;
                wel.remain--;
                wel.free_use -= free_use;
                if(wel.remain==0){
                    wel2stack.push(Wel2(wel.point.first,wel.point.second));
                }
                return;
            }else{
                Wel3 &wel = wel3vec[wel_idx];
                double sc = 0;
                rep(cc,0,3){
                    sc += (wel.col[cc] - p.target[tar].col[cc]) +(wel.col[cc] - p.target[tar].col[cc]);
                }
                sc = sqrtf64(sc);
                query2(ans,wel.point.first,wel.point.second,sc);
                t++;
                wel.remain--;
                wel.free_use -= free_use;
                if(wel.remain==0){
                    wel3stack.push(Wel3(wel.point.first,wel.point.second));
                }
                return;
            }
        };
        auto mix_ink = [&](Answer &ans, int wel_type, int wel_idx, Ink &ink, int add_col, int tar, int free_use) -> void {
            if(wel_type==1){
                // ink.weight = 1, ink.space = 1
                query1(ans,0,0,ink.recipe[0]);
                t++;
                double sc = 0;
                rep(cc,0,3){
                    sc += (ink.col[cc] - p.target[tar].col[cc]) + (ink.col[cc] - p.target[tar].col[cc]);
                }
                sc = sqrtf64(sc);
                query2(ans,0,0,sc);
                t++;
                return;
            }else if(wel_type==2){
                // ink.weight = 2,3, ink.space = 2
                Wel2 &wel = wel2vec[wel_idx];
                query1(ans,wel.point.first,wel.point.second,ink.recipe[0]);
                query1(ans,wel.point.first,wel.point.second,ink.recipe[1]);
                t += 2;
                double sc = 0;
                rep(cc,0,3){
                    sc += (ink.col[cc] - p.target[tar].col[cc]) + (ink.col[cc] - p.target[tar].col[cc]);
                }
                sc = sqrtf64(sc);
                query2(ans,wel.point.first,wel.point.second,sc);
                t++;

                wel.col = ink.sub_col;
                wel.remain = 1;
                wel.free_use = free_use;
                if(ink.add==true){
                    query1(ans,wel.point.first,wel.point.second,add_col);
                    t++;
                    rep(cc,0,3){
                        wel.col[cc] = (ink.sub_col[cc] + p.own[add_col].col[cc]);
                        wel.col[cc] /= 2;
                    }
                    wel.remain++;
                }
            }else if(wel_type==3){
                // ink.weight = 2,3, ink.space = 3
                if(ink.type==2){
                    Wel3 & wel = wel3vec[wel_idx];
                    query1(ans,wel.point.first,wel.point.second,ink.recipe[0]);
                    query1(ans,wel.point.first,wel.point.second,ink.recipe[1]);
                    query1(ans,wel.point.first,wel.point.second,ink.recipe[2]);
                    t += 3;
                    double sc = 0;
                    rep(cc,0,3){
                        sc += (ink.col[cc] - p.target[tar].col[cc]) + (ink.col[cc] - p.target[tar].col[cc]);
                    }
                    sc = sqrtf64(sc);
                    query2(ans,wel.point.first,wel.point.second,sc);
                    t++;

                    wel.col = ink.sub_col;
                    wel.remain = 2;
                    wel.free_use = free_use;
                }else if(ink.type==1){
                    Wel3 &wel = wel3vec[wel_idx];
                    query1(ans,wel.point.first,wel.point.second,ink.recipe[0]);
                    query4(ans,wel.separator[1][0],wel.separator[1][1],wel.separator[1][2],wel.separator[1][3]);
                    query1(ans,wel.point.first,wel.point.second,ink.recipe[1]);
                    t += 3;
                    double sc = 0;
                    rep(cc,0,3){
                        sc += (ink.col[cc] - p.target[tar].col[cc]) + (ink.col[cc] - p.target[tar].col[cc]);
                    }
                    sc = sqrtf64(sc);
                    query2(ans,wel.point.first,wel.point.second,sc);
                    t++;
                    query4(ans,wel.separator[1][0],wel.separator[1][1],wel.separator[1][2],wel.separator[1][3]);
                    t++;

                    wel.col = ink.sub_col;
                    wel.remain = 1;
                    wel.free_use = free_use;
                    if(ink.add==true){
                        query1(ans,wel.point.first,wel.point.second,add_col);
                        t++;
                        rep(cc,0,3){
                            wel.col[cc] = (ink.sub_col[cc] + p.own[add_col].col[cc]);
                            wel.col[cc] /= 2;
                        }
                        wel.remain++;
                    }
                }else{
                    Wel3 &wel = wel3vec[wel_idx];
                    query4(ans,wel.separator[0][0],wel.separator[0][1],wel.separator[0][2],wel.separator[0][3]);
                    query1(ans,wel.point.first+1,wel.point.second,ink.recipe[0]);
                    query4(ans,wel.separator[1][0],wel.separator[1][1],wel.separator[1][2],wel.separator[1][3]);
                    query4(ans,wel.separator[0][0],wel.separator[0][1],wel.separator[0][2],wel.separator[0][3]);
                    query1(ans,wel.point.first,wel.point.second,ink.recipe[1]);
                    t += 5;
                    double sc = 0;
                    rep(cc,0,3){
                        sc += (ink.col[cc] - p.target[tar].col[cc]) + (ink.col[cc] - p.target[tar].col[cc]);
                    }
                    sc = sqrtf64(sc);
                    query2(ans,wel.point.first,wel.point.second,sc);
                    t++;
                    query4(ans,wel.separator[1][0],wel.separator[1][1],wel.separator[1][2],wel.separator[1][3]);
                    t++;

                    wel.col = ink.sub_col;
                    wel.remain = 1;
                    wel.free_use = free_use;
                    if(ink.add==true){
                        query1(ans,wel.point.first,wel.point.second,add_col);
                        t++;
                        rep(cc,0,3){
                            wel.col[cc] = (ink.sub_col[cc] + p.own[add_col].col[cc]);
                            wel.col[cc] /= 2;
                        }
                        wel.remain++;
                    }
                }
            }
        };
        auto trash_ink = [&](Answer &ans, int wel_type, int wel_idx) -> void {
            if(wel_type==1){
                return;
            }else if(wel_type==2){
                Wel2 &wel = wel2vec[wel_idx];
                rep(tt,0,wel.remain){
                    query3(ans,wel.point.first,wel.point.second);
                }
                t += wel.remain;
                wel.remain = 0;
                wel.free_use = 0;
                wel2stack.push(wel);
                return;
            }else{
                Wel3 &wel = wel3vec[wel_idx];
                rep(tt,0,wel.remain){
                    query3(ans,wel.point.first,wel.point.second);
                }
                t += wel.remain;
                wel.remain = 0;
                wel.free_use = 0;
                wel3stack.push(wel);
                return;
            }
        };
        auto calc_eee = [&](array<double,3> &col, CMY &tar) -> double {
            double res = 0;
            rep(cc,0,3){
                res += (col[cc] - tar.col[cc]) + (col[cc] - tar.col[cc]);
            }
            res = sqrtf64(res);
            return res;
        };
        [&]() -> void {
            rep(c,0,H){
                if(simu_reserve[c].type==0){
                    // freeの絵具の中の最小
                    double remain_best_score = 1e9;
                    int remain_best_type = 0;
                    int remain_best_idx = -1;
                    for(auto &w : wel2vec){
                        if(w.free_use == 0) continue;
                        double sc = calc_eee(w.col,p.target[c]);
                        if(sc < remain_best_score){
                            remain_best_score = sc;
                            remain_best_type = 2;
                            remain_best_idx = w.idx;
                        }
                    }
                    for(auto &w : wel3vec){
                        if(w.free_use == 0) continue;
                        double sc = calc_eee(w.col,p.target[c]) * 10000;
                        if(sc < remain_best_score){
                            remain_best_score = sc;
                            remain_best_type = 2;
                            remain_best_idx = w.idx;
                        }
                    }
                    // 新しく色を作る場合の最小
                    double new_best_score = 1e9;
                    int new_best_type = 0;
                    int new_best_idx = -1; // 後から決める
                    Ink new_best_ink;
                    int new_best_add_col;
                    int new_best_free_use = 0;
                    
                    vector<int> simu_reserve_reserve;
                    while(!top3ink[c].empty()){
                        auto pink = top3ink[c].top();
                        top3ink[c].pop();
                        Ink ink = pink.second;
                        double e0 = pink.first;
                        if(ink.weight == 1){
                            double sc = e0 * 10000;
                            if(sc < new_best_score){
                                new_best_score = sc;
                                new_best_type = 1;
                                new_best_ink = ink;
                                new_best_free_use = 0;
                            }
                        }else if(ink.weight == 3){
                            // e1, e2 を求める
                            double e12 = 1e9;
                            vector<int> srr;
                            int fuse = 0;
                            // min(100,(simu_resere[cc].type==0)の個数)
                            int not_reserved = 0;
                            rep(cc,c+1,H){
                                not_reserved += (simu_reserve[cc].type==0);
                                if(not_reserved >= 100) break;
                            }
                            not_reserved = min(100,not_reserved);
                            if(not_reserved==0){
                                e12 = 2*p.D;
                                fuse = 2;
                            }else if(not_reserved==1){
                                rep(cc,c+1,H){
                                    if(simu_reserve[cc].type!=0) continue;
                                    e12 = p.D + calc_eee(ink.sub_col,p.target[cc]) * 10000;
                                    srr.push_back(cc);
                                    fuse = 1;
                                    break;
                                }
                            }else{
                                int best1_sc = 1e9;
                                int best2_sc = 1e9;
                                int best1idx = -1, best2idx = -1;
                                int cnt = 0;
                                rep(cc,c+1,H){
                                    if(simu_reserve[cc].type!=0) continue;
                                    cnt++;
                                    double sc = calc_eee(ink.sub_col,p.target[cc]) * 10000;
                                    if(sc < best1_sc){
                                        best1_sc = sc;
                                        best1idx = cc;
                                    }
                                    if(cnt>=not_reserved) break;
                                }
                                cnt = 1;
                                rep(cc,c+1,H){
                                    if(simu_reserve[cc].type!=0) continue;
                                    if(cc==best1idx) continue;
                                    cnt++;
                                    double sc = calc_eee(ink.sub_col,p.target[cc]) * 10000;
                                    if(sc < best2_sc){
                                        best2_sc = sc;
                                        best2idx = cc;
                                    }
                                    if(cnt>=not_reserved) break;
                                }
                                e12 = best1_sc + best2_sc;
                                srr.push_back(best1idx);
                                srr.push_back(best2idx);
                            }
                            double sc = e0 + e12;
                            sc /= 3;
                            if(sc < new_best_score){
                                new_best_score = sc;
                                new_best_type = 3;
                                simu_reserve_reserve = srr;
                                new_best_ink = ink;
                                new_best_free_use = fuse;
                            }
                        }else{
                            if(ink.space==2){
                                if(ink.add==false){
                                    double e1 = 1e9;
                                    vector<int> srr;
                                    int fuse = 0;
                                    int not_reserved = 0;
                                    rep(cc,c+1,H){
                                        not_reserved += (simu_reserve[cc].type==0);
                                        if(not_reserved >= 15) break;
                                    }
                                    not_reserved = min(15,not_reserved);
                                    if(not_reserved < 1){
                                        e1 = p.D;
                                        fuse = 1;
                                    }else{
                                        int cnt = 0;
                                        int best_idx = -1;
                                        double best1_sc = 1e9;
                                        rep(cc,c+1,H){
                                            if(simu_reserve[cc].type!=0) continue;
                                            cnt++;
                                            double sc = calc_eee(ink.sub_col,p.target[cc]) * 10000;
                                            if(sc < best1_sc){
                                                best1_sc = sc;
                                                best_idx = cc;
                                            }
                                            if(cnt >= not_reserved) break;
                                        }
                                        e1 = best1_sc;
                                        srr.push_back(best_idx);
                                    }
                                    double sc = e0 + e1;
                                    sc /= 2;
                                    if(sc < new_best_score){
                                        new_best_score = sc;
                                        new_best_type = 2;
                                        simu_reserve_reserve = srr;
                                        new_best_ink = ink;
                                        new_best_free_use = fuse;
                                    }
                                }else{
                                    double e12 = 1e9;
                                    vector<int> srr;
                                    int fuse = 0;
                                    int add_col = -1;
                                    array<double,3> new_col;
                                    int not_reserved = 0;
                                    rep(cc,c+1,H){
                                        not_reserved += (simu_reserve[cc].type==0);
                                        if(not_reserved >= 15) break;
                                    }
                                    not_reserved = min(15,not_reserved);
                                    // 追加する色を全て調べる
                                    rep(i,0,p.K){
                                        double ee12 = 1e9;
                                        vector<int> ssrr;
                                        int ffuse = 0;
                                        rep(cc,0,3){
                                            new_col[cc] = (ink.sub_col[cc] + p.own[i].col[cc]);
                                            new_col[cc] /= 2;
                                        }
                                        if(not_reserved == 0){
                                            ee12 = 2*p.D;
                                            ffuse = 2;
                                        }else if(not_reserved == 1){
                                            rep(cc,c+1,H){
                                                if(simu_reserve[cc].type!=0) continue;
                                                ee12 = p.D + calc_eee(new_col,p.target[cc]) * 10000;
                                                ssrr.push_back(cc);
                                                ffuse = 1;
                                                break;
                                            }
                                        }else{
                                            int best1_sc = 1e9;
                                            int best2_sc = 1e9;
                                            int best1idx = -1, best2idx = -1;
                                            int cnt = 0;
                                            rep(cc,c+1,H){
                                                if(simu_reserve[cc].type!=0) continue;
                                                cnt++;
                                                double sc = calc_eee(new_col,p.target[cc]) * 10000;
                                                if(sc < best1_sc){
                                                    best1_sc = sc;
                                                    best1idx = cc;
                                                }
                                                if(cnt>=not_reserved) break;
                                            }
                                            cnt = 1;
                                            rep(cc,c+1,H){
                                                if(simu_reserve[cc].type!=0) continue;
                                                if(cc==best1idx) continue;
                                                cnt++;
                                                double sc = calc_eee(new_col,p.target[cc]) * 10000;
                                                if(sc < best2_sc){
                                                    best2_sc = sc;
                                                    best2idx = cc;
                                                }
                                                if(cnt>=not_reserved) break;
                                            }
                                            ee12 = best1_sc + best2_sc;
                                            ssrr.push_back(best1idx);
                                            ssrr.push_back(best2idx);
                                        }

                                        // ee12の最小
                                        if(ee12 < e12){
                                            e12 = ee12;
                                            add_col = i;
                                            srr = ssrr;
                                            fuse = ffuse;
                                        }
                                    }
                                    double sc = e0 + e12;
                                    sc /= 3;
                                    if(sc < new_best_score){
                                        new_best_score = sc;
                                        new_best_type = 2;
                                        simu_reserve_reserve = srr;
                                        new_best_ink = ink;
                                        new_best_add_col = add_col;
                                        new_best_free_use = fuse;
                                    }
                                }
                            }else{
                                if(ink.add==false){
                                    double e1 = 1e9;
                                    vector<int> srr;
                                    int fuse = 0;
                                    int not_reserved = 0;
                                    rep(cc,c+1,H){
                                        not_reserved += (simu_reserve[cc].type==0);
                                        if(not_reserved >= 100) break;
                                    }
                                    not_reserved = min(100,not_reserved);
                                    if(not_reserved < 1){
                                        e1 = p.D;
                                        fuse = 1;
                                    }else{
                                        int cnt = 0;
                                        int best_idx = -1;
                                        double best1_sc = 1e9;
                                        rep(cc,c+1,H){
                                            if(simu_reserve[cc].type!=0) continue;
                                            cnt++;
                                            double sc = calc_eee(ink.sub_col,p.target[cc]) * 10000;
                                            if(sc < best1_sc){
                                                best1_sc = sc;
                                                best_idx = cc;
                                            }
                                            if(cnt >= not_reserved) break;
                                        }
                                        e1 = best1_sc;
                                        srr.push_back(best_idx);
                                    }
                                    double sc = e0 + e1;
                                    sc /= 2;
                                    if(sc < new_best_score){
                                        new_best_score = sc;
                                        new_best_type = 3;
                                        simu_reserve_reserve = srr;
                                        new_best_ink = ink;
                                        new_best_free_use = fuse;
                                    }
                                }else{
                                    double e12 = 1e9;
                                    vector<int> srr;
                                    int fuse = 0;
                                    int add_col = -1;
                                    array<double,3> new_col;
                                    int not_reserved = 0;
                                    rep(cc,c+1,H){
                                        not_reserved += (simu_reserve[cc].type==0);
                                        if(not_reserved >= 100) break;
                                    }
                                    not_reserved = min(100,not_reserved);
                                    // 追加する色を全て調べる
                                    rep(i,0,p.K){
                                        double ee12 = 1e9;
                                        vector<int> ssrr;
                                        int ffuse = 0;
                                        rep(cc,0,3){
                                            new_col[cc] = (ink.sub_col[cc] + p.own[i].col[cc]);
                                            new_col[cc] /= 2;
                                        }
                                        if(not_reserved == 0){
                                            ee12 = 2*p.D;
                                            ffuse = 2;
                                        }else if(not_reserved == 1){
                                            rep(cc,c+1,H){
                                                if(simu_reserve[cc].type!=0) continue;
                                                ee12 = p.D + calc_eee(new_col,p.target[cc]) * 10000;
                                                ssrr.push_back(cc);
                                                ffuse = 1;
                                                break;
                                            }
                                        }else{
                                            int best1_sc = 1e9;
                                            int best2_sc = 1e9;
                                            int best1idx = -1, best2idx = -1;
                                            int cnt = 0;
                                            rep(cc,c+1,H){
                                                if(simu_reserve[cc].type!=0) continue;
                                                cnt++;
                                                double sc = calc_eee(new_col,p.target[cc]) * 10000;
                                                if(sc < best1_sc){
                                                    best1_sc = sc;
                                                    best1idx = cc;
                                                }
                                                if(cnt>=not_reserved) break;
                                            }
                                            cnt = 1;
                                            rep(cc,c+1,H){
                                                if(simu_reserve[cc].type!=0) continue;
                                                if(cc==best1idx) continue;
                                                cnt++;
                                                double sc = calc_eee(new_col,p.target[cc]) * 10000;
                                                if(sc < best2_sc){
                                                    best2_sc = sc;
                                                    best2idx = cc;
                                                }
                                                if(cnt>=not_reserved) break;
                                            }
                                            ee12 = best1_sc + best2_sc;
                                            ssrr.push_back(best1idx);
                                            ssrr.push_back(best2idx);
                                        }

                                        // ee12の最小
                                        if(ee12 < e12){
                                            e12 = ee12;
                                            add_col = i;
                                            srr = ssrr;
                                            fuse = ffuse;
                                        }
                                    }
                                    double sc = e0 + e12;
                                    sc /= 3;
                                    if(sc < new_best_score){
                                        new_best_score = sc;
                                        new_best_type = 3;
                                        simu_reserve_reserve = srr;
                                        new_best_ink = ink;
                                        new_best_add_col = add_col;
                                        new_best_free_use = fuse;
                                    }
                                }
                            }
                        }
                    }
                    // ウェルの決定
                    if(remain_best_score < new_best_score){
                        simu_reserve[c].type = remain_best_type;
                        simu_reserve[c].idx = remain_best_idx;
                        // use_ink
                        use_ink(ans,simu_reserve[c].type,simu_reserve[c].idx,c,true);
                    }else{
                        bool isEmpty = false;
                        if(new_best_type==1){
                            simu_reserve[c].type = 1;
                            simu_reserve[c].idx = -1;
                        }else if(new_best_type==2){
                            if(wel2stack.empty()){
                                for(auto &w : wel2vec){
                                    if(w.remain==0 || (w.free_use==w.remain)){
                                        //welの中身を廃棄
                                        trash_ink(ans,simu_reserve[c].type, w.idx);
                                        break;
                                    }
                                }
                            }
                            if(wel2stack.empty()){
                                isEmpty = true;
                            }else{
                                simu_reserve[c].type = new_best_type;
                                simu_reserve[c].idx = wel2stack.top().idx;
                                wel2stack.pop();
                            }
                        }else{
                            if(wel3stack.empty()){
                                for(auto &w : wel3vec){
                                    if(w.remain==0 || (w.free_use==w.remain)){
                                        //welの中身を廃棄
                                        trash_ink(ans,simu_reserve[c].type, w.idx);
                                        break;
                                    }
                                }
                            }
                            if(wel3stack.empty()){
                                isEmpty = true;
                            }else{
                                simu_reserve[c].type = new_best_type;
                                simu_reserve[c].idx = wel3stack.top().idx;
                                wel3stack.pop();
                            }
                        }
                        // mix_ink
                        if(isEmpty){
                            mix_ink(ans,1,0,ink_list[0],0,c,0);
                        }else{
                            mix_ink(ans,simu_reserve[c].type,simu_reserve[c].idx,new_best_ink,new_best_add_col,c,new_best_free_use);
                        }
                    }
                }else{
                    //simu_reserve[c].type!=0
                    use_ink(ans,simu_reserve[c].type,simu_reserve[c].idx,c,false);
                }
            }
        }();

        calc_score(ans);
        if(t>p.T){
            ans.score = 1e9;
        }
        if(best_ans.score > ans.score){
            best_ans = ans;
        }
    }
};

int main()
{
    Solver solver;
    // solver.solve_no_mix();
    // solver.solve_1_1_mix();
    // solver.solve_1_1_mix_1_1_1_mix();
    // solver.solve_1_1_mix_1_1_1_mix_reuse();
    solver.solve_froward();
    solver.best_ans.fprint_ans();
}