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

constexpr const int N = 100;
constexpr const int L = 500000;

int T[N];
double T_rate[N];

class TimeKeeper{
    private:
    std::chrono::high_resolution_clock::time_point start_time_;
    int64_t time_threshold_;

    public:
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
};

class Touban {
    short A[N],B[N];
    int seed_;

    public:
    Touban(const int seed) : seed_(seed) {
        this->build_random();
    };

    public:
    void print_ans(){
        rep(i,0,N){
            cout << A[i] << ' ' << B[i] << '\n';
        }
    }

    void build_random(){
        auto mt = mt19937(seed_);
        rep(i,0,N){
            A[i] = mt() % N;
            B[i] = mt() % N;
        }
    }

    int calc_score(){
        int score = 1000000;
        int S[N];
        rep(i,0,N){
            S[i] = 0;
        }
        S[0] = 1;
        int now = 0;
        rep(i,0,L-1){
            if(S[now]&1){
                now = A[now];
                S[now]++;
            }else{
                now = B[now];
                S[now]++;
            }
        }
        int e = 0;
        rep(i,0,N){
            e += abs(S[i]-T[i]);
        }
        return score - e;
    }

    double calc_subscore(){
        double subscore = 0;
        int come[N];
        rep(i,0,N){
            come[A[i]]++;
            come[B[i]]++;
        }
        rep(i,0,N){
            subscore += abs((double)come[i]/(2*N) - T_rate[i]);
        }
        return subscore;
    }
};

bool operator<(Touban &a, Touban &b){
    return 2*a.calc_score() < b.calc_score();
}

int main()
{
    int n,l;
    cin >> n >> l;
    rep(i,0,N){
        cin >> T[i];
        T_rate[i] = (double)T[i]/L;
    }
    auto mt_loop = mt19937(0);
    TimeKeeper tk_main(1900);
    Touban ans = Touban(mt_loop());
    while(!tk_main.isTimeOver()){
        Touban b = Touban(mt_loop());
        if(ans.calc_subscore() > 2*b.calc_subscore()){
            if(ans < b){
                ans = b;
            }
        }
    }
    ans.print_ans();
}