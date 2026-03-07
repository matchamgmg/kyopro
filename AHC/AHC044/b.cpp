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

constexpr const int N = 100;
constexpr const int L = 500000;

struct Problem {
    array<int, N> T;

    void load(){
        int n, l;
        cin >> n >> l;
        rep(i,0,N){
            cin >> T[i];
        }
    }
};

class Solver {
    Problem p;
    mt19937 mt;
    TimeKeeper tk;
    public:
    Solver(){
        mt = mt19937(0);
        p.load();
        tk = TimeKeeper(1900);
    }

    void solve(){
        array<array<int,2>, N> ab;

        auto calc_scroe = [&](){
            int count[N];
            rep(i,0,N) count[i] = 0;
            int at = 0;
            rep(i,0,L) {
                count[at]++;
                at = ab[at][(count[at]&1)^1];// odd -> 0 : even -> 1
            }
            int res = 0;
            rep(i,0,N) res += abs(count[i] - p.T[i]);
            return 1000000 - res;
        };

        int count2[N];
        rep(i,0,N) count2[i] = 0;

        // エルゴード性 : システムの均質性
        auto check_ergodic = [&](){
            // dfs
            bool visited[N];
            rep(i,0,N) visited[i] = false;
            int stack[N];
            int stack_size = 0;
            int visited_count = 0;
            stack[stack_size++] = 0;
            while(stack_size > 0){
                int x = stack[--stack_size];
                if(visited[x]) continue;
                visited[x] = true;
                visited_count++;
                if(!visited[ab[x][0]]) stack[stack_size++] = ab[x][0];
                if(!visited[ab[x][1]]) stack[stack_size++] = ab[x][1];
            }
            int num_positives = 1;
            rep(i,1,N){
                num_positives += count2[i] > 0;
            }
            return visited_count == num_positives;
        };

        int cost = 0;
        rep(i,0,N){
            ab[i][0] = (i+1)%N;
            ab[i][1] = (i+1)%N;
            // 頂点iを通る回数(目標値) -> 頂点iの子頂点を通る回数(近似)
            count2[ab[i][0]] += p.T[i];
            count2[ab[i][1]] += p.T[i];
        }
        rep(i,0,N){
            cost += abs(count2[i] - p.T[i]*2);
        }
        int best_cost = cost;
        auto best = ab;
        /*----------*/
        // climbing
        while(!tk.isTimeOver()){
            // 近傍を求める
            [&](){// swap
                int i = mt()%N;
                int j;
                do{
                    j = mt()%N;
                }while(i==j);
                int pi = mt()%2;
                int pj = mt()%2;
                int vi = ab[i][pi];
                int vj = ab[j][pj];

                int pdiff = abs(count2[vi]-p.T[vi]*2) + abs(count2[vj]-p.T[vj]*2);

                int dij = p.T[j] - p.T[i];
                count2[vi] += dij;
                count2[vj] -= dij;

                int ndiff = abs(count2[vi]-p.T[vi]*2) + abs(count2[vj]-p.T[vj]*2);

                int diff = ndiff - pdiff;
                int ncost = cost + diff;

                if(ncost < cost){
                    ab[i][pi] = vj;
                    ab[j][pj] = vi;

                    if(!check_ergodic()){
                        ab[i][pi] = vi;
                        ab[j][pj] = vj;
                        count2[vi] -= dij;
                        count2[vj] += dij;
                        return;
                    }

                    cost = ncost;
                    if(best_cost > cost){// 焼きなましするときはこれが必要
                        best_cost = cost;
                        best = ab;
                    }
                }else{
                    count2[vi] -= dij;
                    count2[vj] += dij;
                }
            }();

            [&](){
                int i = mt()%N;
                int pa = mt()%2;
                int pv = ab[i][pa];
                int nv = mt()%N;

                int pdiff = abs(count2[pv]-p.T[pv]*2) + abs(count2[nv]-p.T[nv]*2);

                count2[pv] -= p.T[i];
                count2[nv] += p.T[i];

                int ndiff = abs(count2[pv]-p.T[pv]*2) + abs(count2[nv]-p.T[nv]*2);

                int diff = ndiff - pdiff;
                int ncost = cost + diff;

                if(ncost < cost){
                    ab[i][pa] = nv;

                    if(!check_ergodic()){
                        ab[i][pa] = pv;
                        count2[pv] += p.T[i];
                        count2[nv] -= p.T[i];
                        return;
                    }

                    cost = ncost;
                    if(best_cost > cost){
                        best_cost = cost;
                        best = ab;
                    }
                }else{
                    count2[pv] += p.T[i];
                    count2[nv] -= p.T[i];
                }
            }();
        }

        /*----------*/
        ab = best;
        int exact_score = calc_scroe();
        rep(i,0,N){
            cout << ab[i][0] << ' ' << ab[i][1] << '\n';
        }
    }
};

int main()
{
    Solver sol;
    sol.solve();
}