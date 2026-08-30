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

struct Solver {
    Problem p;
    Answer ans;
    mt19937 mt;
    Solver(){
        p.load();
        mt = mt19937(0);
    }

    void query1(int i, int j, int k){
        ans.ans_num.push_back(1);
        ans.ans_query.push_back({i,j,k});
    }

    void query2(int i, int j){
        ans.ans_num.push_back(2);
        ans.ans_query.push_back({i,j});
    }

    void query3(int i, int j){
        ans.ans_num.push_back(3);
        ans.ans_query.push_back({i,j});
    }

    void query4(int i1, int j1, int i2, int j2){
        ans.ans_num.push_back(4);
        ans.ans_query.push_back({i1,j1,i2,j2});
    }

    void struct_separator_4_4(){
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

    void struct_separator_2_1(){
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

    void struct_separator_1_1_2_1_3_1(){
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
        struct_separator_4_4();
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
            ans.ans_num.push_back(2);
            ans.ans_query.push_back({0,0});
        }
    }

    void solve_1_1_mix(){
        struct_separator_2_1();
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
                query2(cm.wel_num.first,cm.wel_num.second);
                cm.remain--;
                if(cm.remain<=0){
                    open_wel.push({cm.wel_num.first,cm.wel_num.second});
                }
            }else{
                if(open_wel.empty()){
                    int ri = (mt()%(N/2))*2;
                    int rj = mt()%N;
                    query3(ri,rj);
                    open_wel.push({ri,rj});
                    auto code = code_map[ri][rj];
                    ColorManage &rcm = pallet[code.first][code.second];
                    rcm.remain = 0;
                }
                auto wel = open_wel.top();
                open_wel.pop();
                query1(wel.first,wel.second,best_code.first);
                if(best_code.first!=best_code.second){
                    query1(wel.first,wel.second,best_code.second);

                }
                query2(wel.first,wel.second);
                if(best_code.first==best_code.second){
                    open_wel.push(wel);
                }else{
                    cm.remain = 1;
                    cm.wel_num = wel;
                    code_map[wel.first][wel.second] = best_code;
                }
            }
        }
    }

    void solve_1_1_mix_1_1_1_mix(){
        // separator
        struct_separator_1_1_2_1_3_1();
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
                            col.col[c] /= 3;
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
                query2(cm.wel_num.first,cm.wel_num.second);
                cm.remain--;
                if(cm.remain <= 0){
                    (col_dim==1 ? open_wel_1 : col_dim==2 ? open_wel_2 : open_wel_3).push(cm.wel_num);
                }
            }else{
                if(col_dim==1){
                    if(open_wel_1.empty()){
                        int ri = mt()%N;
                        auto rcode = wel2code[0][ri];
                        ColorManage &rcm = pallet[rcode[0]][rcode[1]][rcode[2]];
                        rep(i,0,rcm.remain){
                            query3(rcm.wel_num.first,rcm.wel_num.second);
                        }
                        rcm.remain = 0;
                        open_wel_1.push(rcm.wel_num);
                    }
                    auto wel_address = open_wel_1.top();
                    open_wel_1.pop();
                    query1(wel_address.first,wel_address.second,best_code[0]);
                    query2(wel_address.first,wel_address.second);
                    open_wel_1.push(wel_address);
                }else if(col_dim==2){
                    if(open_wel_2.empty()){
                        int ri,rj;
                        int vv;
                        do{
                            ri = mt()%8;
                            rj = mt()%N;
                            auto v = wel2code[ri][rj];
                            vv = (v[0]<0) + (v[1]<0) + (v[2]<0);
                        }while(ri==0 || vv==3);
                        auto rcode = wel2code[ri][rj];
                        ColorManage &rcm = pallet[rcode[0]][rcode[1]][rcode[2]];
                        rep(i,0,rcm.remain){
                            query3(rcm.wel_num.first,rcm.wel_num.second);
                        }
                        rcm.remain = 0;
                        open_wel_2.push(rcm.wel_num);
                    }
                    auto wel_address = open_wel_2.top();
                    open_wel_2.pop();
                    query1(wel_address.first,wel_address.second,best_code[0]);
                    query1(wel_address.first,wel_address.second,best_code[1]);
                    query2(wel_address.first,wel_address.second);
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
                        }while(ri<8 || vv==3);
                        auto rcode = wel2code[ri][rj];
                        ColorManage &rcm = pallet[rcode[0]][rcode[1]][rcode[2]];
                        rep(i,0,rcm.remain){
                            query3(rcm.wel_num.first,rcm.wel_num.second);
                        }
                        rcm.remain = 0;
                        open_wel_3.push(rcm.wel_num);
                    }
                    auto wel_address = open_wel_3.top();
                    open_wel_3.pop();
                    query1(wel_address.first,wel_address.second,best_code[0]);
                    query1(wel_address.first,wel_address.second,best_code[1]);
                    query1(wel_address.first,wel_address.second,best_code[2]);
                    query2(wel_address.first,wel_address.second);
                    cm.remain = 2;
                    cm.wel_num = wel_address;
                    wel2code[wel_address.first][wel_address.second] = best_code;
                }
            }
        }
    }
};

int main()
{
    Solver solver;
    solver.solve_1_1_mix_1_1_1_mix();
    solver.ans.print_ans();
}