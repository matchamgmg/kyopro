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

vector<int> dx = {0,1,0,-1}, dy = {1,0,-1,0};
// [R, D, L, U]
vector<char> dc = {'.', '>', 'v', '<', '^'};
vector<char> dcc = {'S', 'R', 'D', 'L', 'U'};

struct Color {
    int state, dc_index;
    bool is_objective;
    int color_number;

    Color () {}
    Color (int s, int i, bool b, int n) : state(s), dc_index(i), is_objective(b), color_number(n) {}
};

bool operator<(const Color &a, const Color &b){
    return a.color_number < b.color_number;
}

struct Rule {
    int state, next_state;
    Color color, next_color;
    int dc_index; 
};

struct Answer {
    int N;
    int C, Q, M;
    vector<int> s;
    vector<int> c, q, A, S;
    vector<char> D;
    Answer () {}
    Answer (int n_) : N(n_), C(0), Q(0), M(0) {
        s.resize(N*N,0);
    }
    void add_c(){
        C++;
    }
    void add_q(){
        Q++;
    }
    void add_rule(int col, int state, int nxt_c, int nxt_s, char direction){
        c.push_back(col);
        q.push_back(state);
        A.push_back(nxt_c);
        S.push_back(nxt_s);
        D.push_back(direction);
        M++;
    }
    void print_ans(){
        cout << C << ' ' << Q << ' ' << M << '\n';
        rep(i,0,N){
            rep(j,0,N){
                cout << s[i*N+j] << " \n"[j==N-1];
            }
        }
        rep(i,0,M){
            cout << c[i] << ' ' << q[i] << ' ' << A[i] << ' ' << S[i] << ' ' << D[i] << '\n';
        }
    }
    void fprint_ans(){
        ofstream ss("out.txt");
        ss << C << ' ' << Q << ' ' << M << '\n';
        rep(i,0,N){
            rep(j,0,N){
                ss << s[i*N+j] << " \n"[j==N-1];
            }
        }
        rep(i,0,M){
            ss << c[i] << ' ' << q[i] << ' ' << A[i] << ' ' << S[i] << ' ' << D[i] << '\n';
        }
    }
};

struct Solver {
    Answer ans;
    int N, K, T;
    vector<vector<bool>> field;
    vector<int> objective_map, objective_vec;
    vector<vector<int>> multi_layer_field;
    map<pair<Color,int>,tuple<Color,int,int>> Rules;
    vector<Color> initial_color_map;
    Solver () {
        cin >> N >> K >> T;
        ans = Answer(N);
        field.resize(N*N);
        initial_color_map.resize(N*N);
        objective_map.resize(N*N,0);
        objective_vec.resize(K,0);
        rep(i,0,N*N){
            field[i].resize(4,false);
        }
        string s;
        rep(i,0,N){
            cin >> s;
            rep(j,0,N-1){
                field[i*N+j][0] = (s[j] == '0');
                field[i*N+j+1][2] = (s[j] == '0');
            }
        }
        rep(i,0,N-1){
            cin >> s;
            rep(j,0,N){
                field[i*N+j][1] = (s[j] == '0');
                field[(i+1)*N+j][3] = (s[j] == '0');
            }
        }
        int x, y;
        rep(i,0,K){
            cin >> x >> y;
            objective_map[x*N+y] = i+1;
            objective_vec[i] = x*N+y;
        }
        multi_layer_field.resize(K-1);
        rep(i,0,K-1){
            multi_layer_field[i].resize(N*N,0);
        }
        K--;
    }

    void encode_answer(Answer &ans_, int color_number_, vector<Color> &init_color_, map<pair<Color,int>,tuple<Color,int,int>> &rules_){
        ans_.C = color_number_;
        ans.Q = K;
        ans.M = rules_.size();
        rep(i,0,N){
            rep(j,0,N){
                ans.s[i*N+j] = init_color_[i*N+j].color_number;
            }
        }
        for(auto p : rules_){
            ans_.c.push_back(p.first.first.color_number);
            ans_.q.push_back(p.first.second);
            ans_.A.push_back(get<0>(p.second).color_number);
            ans_.S.push_back(get<1>(p.second));
            ans_.D.push_back(dcc[get<2>(p.second)]);
        }
    }

    void BFS_backtrack(){
        int s = 0;
        int g = 0;
        const int INF = N*(N+1);
        vector<int> dist;
        rep(i,0,K){
            s = objective_vec[i];
            g = objective_vec[i+1];
            dist.assign(N*N,INF);
            queue<int> que;
            que.push(s);
            dist[s] = 0;
            bool flag = false;
            while(!que.empty() && !flag){
                int u = que.front();
                que.pop();
                rep(j,0,4){
                    if(field[u][j]==false) continue;
                    int v = u + dx[j]*N + dy[j];
                    if(dist[v]<INF) continue;
                    que.push(v);
                    dist[v] = dist[u] + 1;
                    if(v==g){
                        flag = true;
                        break;
                    }
                }
            }
            int u = g;
            while(u!=s){
                rep(j,0,4){
                    if(field[u][j]==false) continue;
                    int v = u + dx[j]*N + dy[j];
                    if(dist[u]==dist[v]+1){
                        multi_layer_field[i][v] = (j+2)%4 + 1;    //[., >, v, <, ^]
                        u = v;
                        break;
                    }
                }
            }
            // rep(ii,0,N){
            //     rep(jj,0,N){
            //         cout << dc[multi_layer_field[i][ii*N+jj]] << " \n"[jj==N-1];
            //     }
            // }
            // cout << '\n';
        }
    }

    void from_bottom_layer(){
        // mp[state,dc_index]
        vector<vector<vector<Color>>> mp(K,vector<vector<Color>>(5,vector<Color>()));
        int color_number = 0;
        Color base_color = Color(K-1,0,false,color_number);
        mp[K-1][0].push_back(base_color);
        color_number++;
        rep(i,0,N){
            rep(j,0,N){
                int k = K-1;
                // 最下層について
                Color n_color;
                bool exist = false;
                for(Color &col : mp[k][multi_layer_field[k][i*N+j]]){
                    if(col.is_objective==(objective_vec[k]==i*N+j)){
                        exist = true;
                        n_color = col;
                        break;
                    }
                }
                if(!exist){
                    n_color = Color(k,multi_layer_field[k][i*N+j],(objective_vec[k]==i*N+j),color_number);
                    mp[k][multi_layer_field[k][i*N+j]].push_back(n_color);
                    color_number++;
                    // 新しい色から基底色への遷移規則
                    Rules[{n_color,k-(n_color.is_objective)}] = {base_color,k,n_color.dc_index};
                }
                // 上の階層を見ていく
                // その階層のgoalは無視 goalはdc_index==0
                // その階層のstartを考慮 startはdc_index!=0
                int nk = k;
                k--;
                while(k>=0){
                    while(k>=0 && multi_layer_field[k][i*N+j]==0) k--;
                    if(k<0){
                        // n_colorが初期色になる
                        initial_color_map[i*N+j] = n_color;
                    }else{
                        // p_color -> n_color
                        Color p_color = Color(k,multi_layer_field[k][i*N+j],(k!=0 && objective_vec[k]==i*N+j),-1);
                        exist = false;
                        for(Color &col : mp[k][multi_layer_field[k][i*N+j]]){
                            if(col.is_objective==p_color.is_objective){
                                exist = true;
                                p_color.color_number = col.color_number;
                                break;
                            }
                        }
                        if(exist){
                            // 遷移規則がすでにあるか
                            if(!Rules.count({p_color,k})){
                                Rules[{p_color,k}] = {n_color,nk,p_color.dc_index};
                            }else{
                                Color to_color = get<0>(Rules[{p_color,k}]);
                                if(to_color.color_number!=n_color.color_number){
                                    p_color.color_number = color_number;
                                    color_number++;
                                    Rules[{p_color,k}] = {n_color,k,p_color.dc_index};
                                }
                            }
                        }else{
                            // objectiveは必ずこっち
                            p_color.color_number = color_number;
                            color_number++;
                            // 新しい色を登録
                            mp[k][multi_layer_field[k][i*N+j]].push_back(p_color);
                            // 新しい遷移規則を追加
                            Rules[{p_color,(k-(p_color.is_objective))}] = {n_color,k,p_color.dc_index};
                        }
                        if(k==0){
                            // 初期色になる
                            initial_color_map[i*N+j] = p_color;
                        }
                        nk = k;
                        k--;
                        n_color = p_color;
                    }
                }
            }
        }
        // Answer を encode
        encode_answer(ans,color_number,initial_color_map,Rules);
    }

    void simple_decision(){
        vector<vector<tuple<int,int,char>>> simple_rule(N*N,vector<tuple<int,int,char>>(K,{-1,-1,'.'}));
        vector<int> rule_cnt(N*N,0);
        int layer = 0;
        int pos = objective_vec[0];
        int d = 0;
        while(layer<K){
            while(grid_check(pos/N,pos%N,N,N) && multi_layer_field[layer][pos]!=0){
                d = multi_layer_field[layer][pos];
                simple_rule[pos][layer] = {pos,layer,dcc[d]};
                rule_cnt[pos]++;
                pos += dx[d-1]*N + dy[d-1];
            }
            if(layer<K-1){
                d = multi_layer_field[layer+1][pos];
                simple_rule[pos][layer] = {pos,layer+1,dcc[d]};
                rule_cnt[pos]++;
                pos += dx[d-1]*N + dy[d-1];
            }
            layer++;
        }
        int C = 0;
        int C_minus = 0;
        int Q = K;
        int M = 0;
        int nc, nq;
        char nd;
        rep(i,0,N*N){
            if(rule_cnt[i]>0){
                // i - C_minus == C
                ans.s[i] = C;
                rep(j,0,K){
                    tie(nc,nq,nd) = simple_rule[i][j];
                    if(nc==-1) continue;
                    ans.add_rule(nc-C_minus,j,nc-C_minus,nq,nd);
                    M++;
                }
                C++;
            }else{
                C_minus++;
                ans.s[i] = 0;
            }
        }
        ans.C = C;
        ans.Q = Q;
        ans.M = M;
    }

    void solve(){
        BFS_backtrack();
        // rep(i,0,K){
        //     cout << multi_layer_field[i][12*N+3];
        // }
        // cout << endl;
        // from_bottom_layer();
        simple_decision();
        ans.fprint_ans();
    }
};

int main()
{
    Solver sol;
    sol.solve();
}