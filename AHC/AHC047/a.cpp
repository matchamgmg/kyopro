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

constexpr const int N = 36;
constexpr const int M = 12;
constexpr const int L = 1000000;

void random_ans(int M, vector<char> &C, vvi &A){
    mt19937 mt(0);
    vector<int> R(M);
    rep(i,0,M){
        int sum = 0;
        C[i] = mt()%6 + 'a';
        rep(j,0,M){
            R[j] = mt()%1000;
            sum += R[j];
        }
        int sum2 = 0;
        rep(j,0,M-1){
            A[i][j] = 100 * R[j] / sum;
            sum2 += A[i][j];
        }
        A[i][M-1] = 100 - sum2;
        // check
        bool ok = true;
        rep(j,0,M){
            ok &= A[i][j]>=0;
        }
        if(!ok){
            i--;
            continue;
        }
    }
}

void print_ans(int M, vector<char> &C, vvi &A){
    rep(i,0,M){
        cout << C[i] << ' ';
        rep(j,0,M){
            cout << A[i][j] << " \n"[j==M-1];
        }
    }
}

void greedy_ans(int N, int M, vector<char> &C, vvi &A, vector<string> &S, vector<ll> &P){
    vi idx(N);
    iota(all(idx),0);
    sort(all(idx),[&](int i, int j){ return P[i]>P[j]; });
    rep(i,0,S[idx[0]].size()){
        C[i] = S[idx[0]][i];
    }
    rep(i,0,M){
        if(C[i]==0){
            C[i] = 'a';
            A[i][0] = 100;
        }
    }
    rep(i,0,S[idx[0]].size()){
        A[i][(i+1)%S[idx[0]].size()] = 100;
    }
    // print_ans(M,C,A);
}

void nazo_ans(int N, int M, vector<char> &C, vvi &A, vector<string> &S, vector<ll> &P){
    return;
}

void count_weighted_freq(int N, vector<string> &S, vector<ll> &P,vvll &wcnt){
    rep(i,0,N){
        rep(j,0,S[i].size()-1){
            wcnt[S[i][j]-'a'][S[i][j+1]-'a'] += P[i];
        }
    }
}

ll calc_score(int N, int M, vector<char> &C, vvi &A, vector<string> &S, vll &P){
    ll res = 0;
    vvi table(6,vi());
    rep(i,0,M){
        table[C[i]-'a'].push_back(i);
    }

    ll prob = 1;
    auto dfs = [&](auto f, string &s, int c=0, ll nscore=1, int step=0) -> void {
        if(step==s.size()){
            prob += nscore;
            return;
        }
        if(nscore==0) return;
        for(int next : table[C[c]-'a']){
            f(f,s,next,nscore*A[c][next],step+1);
        }
        return;
    };

    rep(i,0,N){
        prob = 1;
        dfs(dfs,S[i]);
        res += prob*P[i];
    }

    return res;
}

void climbing(int N, int M, vector<char> &C, vvi &A, vector<string> &S, vector<ll> &P){
    greedy_ans(N,M,C,A,S,P);

    // climbing
    vector<char> best_C(M);
    vvi best_A(M,vi(M));
    best_C = C;
    best_A = A;
    ll best_score = calc_score(N,M,best_C,best_A,S,P);
    TimeKeeper tk(1900);
    mt19937 mt(0);
    set<int> used;
    while(!tk.isTimeOver()){
        // [&](){
        //     int pc = C[0]-'a';
        //     int nc = mt()%6;
        //     if(used.count(nc)) return;
        //     else used.insert(nc);
        //     C[0] = nc + 'a';
        //     ll nscore = calc_score(N,M,C,A,S,P);
        //     if(best_score > nscore){
        //         C[0] = pc+'a';
        //     }else{
        //         best_score = nscore;
        //         best_C = C;
        //         best_A = A;
        //     }
        // }();

        [&](){
            int i = mt()%M;
            int pj = mt()%M;
            int nj;
            do{
                nj = mt()%M;
            }while(nj==pj);
            int m = mt()%(A[i][pj]+1);
            A[i][pj] -= m;
            A[i][nj] += m;
            ll nscore = calc_score(N,M,C,A,S,P);
            if(best_score > nscore){
            A[i][pj] += m;
            A[i][nj] -= m;
            }else{
                best_score = nscore;
                best_C = C;
                best_A = A;
            }
        };
    }

    print_ans(M,C,A);
}

template<int char_size, int base>
struct Trie{
    struct Node{
        vector<int> next;
        vector<int> accept;
        int c;
        int common;
        Node(int c_) : c(c_), common(0) {
            next.assign(char_size,-1);
        }
    };
    vector<Node> nodes;
    int root;
    Trie() : root(0){
        nodes.push_back(Node(root));
    }

    void insert(const string &word, int word_id){
        int node_id = 0;
        for(int i = 0; i < (int)word.size(); i++){
            int c = (int)(word[i]-base);
            int &next_id = nodes[node_id].next[c];
            if(next_id == -1){
                next_id = (int)nodes.size();
                nodes.push_back(Node(c));
            }
            ++nodes[node_id].common;
            node_id = next_id;
        }
        ++nodes[node_id].common;
        nodes[node_id].accept.push_back(word_id);
    }
    void insert(const string &word){
        insert(word,nodes[0].common);
    }

    vector<int> search(const string &word, bool prefix = true){
        vector<int> res;
        int node_id = 0;
        for(int i = 0; i < (int)word.size(); i++){
            int c = (int)(word[i]-base);
            int &next_id = nodes[node_id].next[c];
            if(next_id == -1){
                return vector<int>();
            }
            node_id = next_id;
            for(int num : nodes[next_id].accept){
                res.push_back(num);
            }
        }
        return (nodes[node_id].accept.size() > 0 ? res : vector<int>());
    }
};

using ull = unsigned long long;
const ull MASK30 = (1ULL << 30) - 1;
const ull MASK31 = (1ULL << 31) - 1;
const ull MOD = (1ULL << 61) - 1;
const ull MASK61 = MOD;
const ull POSITIVIZER = MOD*4;

ull Mull(ull a, ull b){
    ull au = a >> 31;
    ull ad = a & MASK31;
    ull bu = b >> 31;
    ull bd = b & MASK31;
    ull mid = ad * bu + au *bd;
    ull midu = mid >> 30;
    ull midd = mid & MASK30;
    return au*bu*2 + midu + (midd<<31) + ad*bd;
    //return CalcMod(au*bu*2 + midu + (midd<<31) + ad*bd);
}

/*mod 2^61-1を計算する関数*/
ull CalcMod(ull x){
    ull xu = x >> 61;
    ull xd = x & MASK61;
    ull res = xu + xd;
    if(res>=MOD) res -= MOD;
    return res;
}

ull GCD(ull a, ull b){
    if(a%b==0) return b;
    return GCD(b,a%b);
}

ull PowMod(ull base, ull exp){
    ull res = 1;
    while(exp>0){
        if(exp&1) res = CalcMod(Mull(res,base));
        exp >>= 1;
        base = CalcMod(Mull(base,base));
    }
    return res;
}

ull RandBase(ull base0, ull A, mt19937_64 &mt){
    ull res;
    ull k;
    do{
        k = mt()%(MOD-1);
        if(GCD(k,MOD-1)!=1) continue;
        res = PowMod(base0,k);
    }while(res<=A);
    return res;
}

int main()
{
    int n,m,l;
    cin >> n >> m >> l;
    vector<string> S(N);
    vector<ll> P(N);
    rep(i,0,N){
        cin >> S[i] >> P[i];
    }
    // Trie<6,(int)'a'> tr;
    // rep(i,0,N){
    //     tr.insert(S[i]);
    // }
    // vector<ll> nP(N,0);
    // rep(i,0,N){
    //     vi idx = tr.search(S[i]);
    //     for(int j : idx){
    //         nP[i] += P[j];
    //     }
    // }
    // rep(i,0,N){
    //     P[i] = nP[i];
    // }

    // RollingHash
    vll nP(N,0);
    mt19937_64 mt(0);
    ull base0 = 37;
    ull base = RandBase(base0,100,mt);
    vector<ull> powMemo (N+1,0);
    powMemo[0] = 1;
    rep(i,0,20){
        powMemo[i+1] = CalcMod(Mull(powMemo[i],base));
    }
    vector<vector<ull>> hash(N);
    rep(i,0,N){
        vector<ull> hash_((int)S[i].size()+1,0);
        rep(j,0,(int)S[i].size()){
            hash_[j+1] = CalcMod(Mull(hash_[j],base) + S[i][j]);
        }
        hash[i] = hash_;
    }
    rep(ii,0,N-1){
        rep(jj,ii+1,N){
            bool flag = false;
            int i = ii;
            int j = jj;
            if(S[i].size()<S[j].size()) swap(i,j);
            ull hash_j = hash[j][S[j].size()];
            ull hash_i;
            rep(k,0,(int)S[i].size()-(int)S[j].size()+1){
                hash_i = CalcMod(hash[i][k+(int)S[j].size()] + POSITIVIZER - Mull(hash[i][k],powMemo[(int)S[j].size()]));
                if(hash_i==hash_j){
                    if(!flag){
                        nP[i] += P[j];
                        flag = true;
                    }
                }
            }
        }
    }

    rep(i,0,N){
        P[i] += nP[i];
    }

    vector<char> C(M,0);
    vvi A(M,vi(M,0));

    climbing(N,M,C,A,S,P);
    // greedy_ans(N,M,C,A,S,P);
    // print_ans(M,C,A);
}