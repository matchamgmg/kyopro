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
    mt19937 mt;
    Answer ans;
    int N, K, T;
    vector<vector<bool>> field;
    vector<int> objective_map, objective_vec;
    vector<vector<int>> multi_layer_field;
    int state_num;
    Solver () {
        cin >> N >> K >> T;
        mt = mt19937(0);
        ans = Answer(N);
        field.resize(N*N);
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
        state_num = K;
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

    void struct_highway(){

    }

    vector<int> compress_state(){
        // layer -> state
        int nK = 0;
        vector<int> layer_to_state(K);
        iota(all(layer_to_state),0);
        dsu uf(K);
        vector<vector<int>> layers(K,vector<int>(N*N));
        rep(layer,0,K){
            rep(i,0,N){
                rep(j,0,N){
                    layers[layer][i*N+j] = multi_layer_field[layer][i*N+j];
                }
            }
        }
        vector<set<int>> G_set(K);
        vector<vector<int>> G_vec(K);
        vector<bool> used(K,false);
        bool have_edge, check_ok;
        bool finish = false;
        while(!finish){
            finish = true;
            // make graph
            rep(i,0,K){
                have_edge = false;
                G_vec[i].clear();
                G_set[i].clear();
                if(used[i]) continue;
                rep(j,0,K){
                    if(i==j) continue;
                    if(used[j]) continue;
                    if(i>j){
                        if(G_set[j].count(i)){
                            G_set[i].insert(j);
                            G_vec[i].push_back(j);
                            have_edge = true;
                        }
                        continue;
                    }
                    // make edge
                    check_ok = true;
                    rep(x,0,N*N){
                        int d1 = layers[i][x];
                        int d2 = layers[j][x];
                        if(d1!=0 && d2!=0){
                            check_ok = false;
                            break;
                        }
                    }
                    if(check_ok){
                        G_vec[i].push_back(j);
                        G_set[i].insert(j);
                        have_edge = true;
                    }
                }
                if(have_edge) finish = false;
            }
            // random merge
            rep(i,0,K){
                if(used[i]) continue;
                int rand_max = G_vec[i].size();
                if(rand_max==0){
                    used[i] = true;
                    // push to answer
                    // cout << nK << ": ";
                    rep(j,0,K){
                        if(uf.same(i,j)){
                            layer_to_state[j] = nK;
                            // cout << j << ' ';
                        }
                    }
                    // cout << '\n';
                    nK++;
                    // rep(x,0,N*N){
                    //     cout << dc[layers[i][x]] << " \n"[x%N==N-1];
                    // }
                    // cout << '\n';
                    continue;
                }
                int rand = mt()%rand_max;
                int j = G_vec[i][rand];
                if(!G_set[j].count(i)) continue;
                G_set[i].clear();
                G_set[j].clear();
                used[j] = true;
                uf.merge(i,j);
                rep(x,0,N*N){
                    if(layers[i][x]==0){
                        layers[i][x] = layers[j][x];
                    }
                }
            }
        }
        state_num = nK;
        return layer_to_state;
    }

    Answer compress_color(Answer &ans_){
        // vector<pair<int,char>> // state -> state, direction の重複を調べる
        int Q = ans_.Q;
        int C = ans_.C;
        Answer res = Answer(N);
        vector<int> p_col_to_n_col(C);
        iota(all(p_col_to_n_col),0);
        dsu uf(C);
        int nC = 0;
        int nM = 0;
        vector<vector<tuple<int,int,char>>> rules(C,vector<tuple<int,int,char>>(Q,{-1,-1,'.'}));
        vector<vector<int>> G_vec(C);
        vector<set<int>> G_set(C);
        vector<bool> used(C,false);
        int r1,r2,r3,r4;
        char r5;
        int s1,s2,s3,s4;
        char s5;
        rep(i,0,ans_.M){
            r1 = ans_.c[i];
            r2 = ans_.q[i];
            r3 = ans_.A[i];
            r4 = ans_.S[i];
            r5 = ans_.D[i];
            rules[r1][r2] = {r3,r4,r5};
        }
        bool finish = false;
        bool have_edge;
        bool check_ok;
        while(!finish){
            finish = true;
            // make graph
            rep(i,0,C){
                have_edge = false;
                G_vec[i].clear();
                G_set[i].clear();
                if(used[i]) continue;
                rep(j,0,C){
                    if(i==j) continue;
                    if(used[j]) continue;
                    if(i>j){
                        if(G_set[j].count(i)){
                            G_set[i].insert(j);
                            G_vec[i].push_back(j);
                            have_edge = true;
                        }
                        continue;
                    }
                    // make edge
                    check_ok = true;
                    rep(k,0,Q){
                        tie(ignore,r4,r5) = rules[i][k];
                        tie(ignore,s4,s5) = rules[j][k];
                        if(r4!=-1 && s4!=-1 && (r4!=s4 || r5!=s5)){
                            check_ok = false;
                            break;
                        }
                    }
                    if(check_ok){
                        G_vec[i].push_back(j);
                        G_set[i].insert(j);
                        have_edge = true;
                    }
                }
                if(have_edge) finish = false;
            }
            // random merge
            rep(i,0,C){
                if(used[i]) continue;
                int rand_max = G_vec[i].size();
                if(rand_max==0){
                    used[i] = true;
                    // push to answer
                    rep(j,0,C){
                        if(uf.same(i,j)){
                            p_col_to_n_col[j] = nC;
                        }
                    }
                    rep(k,0,Q){
                        tie(ignore,r4,r5) = rules[i][k];
                        if(r4==-1) continue;
                        res.add_rule(nC,k,nC,r4,r5);
                        nM++;
                    }
                    nC++;
                    continue;
                }
                int rand = mt()%rand_max;
                int j = G_vec[i][rand];
                if(!G_set[j].count(i)) continue;
                G_set[i].clear();
                G_set[j].clear();
                used[j] = true;
                uf.merge(i,j);
                rep(k,0,Q){
                    tie(ignore,r4,r5) = rules[i][k];
                    if(r4==-1){
                        rules[i][k] = rules[j][k];
                    }
                }
            }
        }
        res.C = nC;
        res.Q = Q;
        res.M = nM;
        rep(i,0,N){
            rep(j,0,N){
                res.s[i*N+j] = p_col_to_n_col[ans_.s[i*N+j]];
            }
        }
        return res;
    }

    void improved_simple_decision(){
        vector<vector<tuple<int,int,int,int,char>>> simple_rule(N*N,vector<tuple<int,int,int,int,char>>());
        vector<int> rule_cnt(N*N,0);
        vector<int> f(N*N,-1);
        int layer = 0;
        int pos = objective_vec[0];
        int d = 0;
        int q = 0;
        int ppos;
        int r1,r2,r3,r4;
        char r5;
        while(layer<K){
            while(grid_check(pos/N,pos%N,N,N) && multi_layer_field[layer][pos]!=0){
                if(f[pos]==q){
                    tie(r1,r2,r3,r4,r5) = simple_rule[ppos][rule_cnt[ppos]-1];
                    simple_rule[ppos][rule_cnt[ppos]-1] = {r1,r2,r3,r4+1,r5};
                    q++;
                }
                d = multi_layer_field[layer][pos];
                simple_rule[pos].push_back({pos,q,pos,q,dcc[d]});
                rule_cnt[pos]++;
                f[pos] = q;
                ppos = pos;
                pos += dx[d-1]*N + dy[d-1];
            }
            if(layer<K-1){
                if(f[pos]==q){
                    tie(r1,r2,r3,r4,r5) = simple_rule[ppos][rule_cnt[ppos]-1];
                    simple_rule[ppos][rule_cnt[ppos]-1] = {r1,r2,r3,r4+1,r5};
                    q++;
                }
                d = multi_layer_field[layer+1][pos];
                simple_rule[pos].push_back({pos,q,pos,q,dcc[d]});
                rule_cnt[pos]++;
                f[pos] = q;
                ppos = pos;
                pos += dx[d-1]*N + dy[d-1];
            }
            layer++;
        }
        int C = 0;
        int C_minus = 0;
        int Q = q+1;
        int M = 0;
        rep(i,0,N*N){
            if(rule_cnt[i]>0){
                // i - C_minus == C
                ans.s[i] = C;
                rep(j,0,rule_cnt[i]){
                    tie(r1,r2,r3,r4,r5) = simple_rule[i][j];
                    ans.add_rule(r1-C_minus,r2,r3-C_minus,r4,r5);
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

    void compressed_state_based_decision(){
        vector<int> layer_to_state = compress_state();
        set<int> state_set;
        for(int x : layer_to_state){
            state_set.insert(x);
        }
        int siz = state_set.size();
        vector<int> trans_state(siz);
        iota(all(trans_state),0);
        {
            int t = layer_to_state[0];
            swap(trans_state[t],trans_state[0]);
        }
        vector<vector<tuple<int,int,int,int,char>>> simple_rule(N*N,vector<tuple<int,int,int,int,char>>());
        vector<int> rule_cnt(N*N,0);
        vector<int> f(N*N,-1);
        int layer = 0;
        int pos = objective_vec[0];
        int npos = 0;
        int d = 0;
        int nd = 0;
        int r1,r2,r3,r4;
        char r5;
        while(layer<K){
            d = multi_layer_field[layer][pos];
            npos = pos + dx[d-1]*N + dy[d-1];
            nd = multi_layer_field[layer][npos];
            if(nd!=0){
                simple_rule[pos].push_back({pos,layer,pos,layer,dcc[d]});
            }else{
                simple_rule[pos].push_back({pos,layer,pos,layer+(layer<K-1),dcc[d]});
                layer++;
            }
            rule_cnt[pos]++;
            pos = npos;
        }
        int C = 0;
        int C_minus = 0;
        int Q = state_set.size();
        int M = 0;
        rep(i,0,N*N){
            if(rule_cnt[i]>0){
                // i - C_minus == C
                ans.s[i] = C;
                rep(j,0,rule_cnt[i]){
                    tie(r1,r2,r3,r4,r5) = simple_rule[i][j];
                    ans.add_rule(r1-C_minus,trans_state[layer_to_state[r2]],r3-C_minus,trans_state[layer_to_state[r4]],r5);
                    M++;
                }
                C++;
            }else{
                C_minus++;
                ans.s[i] = 0;
            }
        }
        ans.C = C;
        // ans.Q = K;
        ans.Q = Q;
        ans.M = M;
    }

    void solve(){
        BFS_backtrack();
        compressed_state_based_decision();
        ans = compress_color(ans);
        ans.print_ans();
    }
};

int main()
{
    Solver sol;
    sol.solve();
}