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
vector<set<int>> grade(5);

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
        this->build_by_grade();
    };

    public:
    void print_ans(){
        rep(i,0,N){
            cout << A[i] << ' ' << B[i] << '\n';
        }
    }

    void build_by_grade(){
        int head_tail[5][2];
        rep(i,0,5)rep(j,0,2) head_tail[i][j] = -1;
        rep(i,0,5){
            queue<int> q;
            for(int a : grade[i]){
                q.push(a);
            }
            while(!q.empty()){
                int u = q.front();
                if(head_tail[i][0]==-1){
                    head_tail[i][0] = u;
                }
                q.pop();
                if(q.empty()){
                    head_tail[i][1] = u;
                    A[u] = head_tail[i][0];
                    B[u] = A[u];
                }else{
                    A[u] = q.front();
                    B[u] = A[u];
                }
            }
        }
        A[head_tail[0][1]] = head_tail[1][0];
        A[head_tail[1][1]] = head_tail[2][0];
        A[head_tail[2][1]] = head_tail[3][0];
        A[head_tail[3][1]] = head_tail[0][0];
        A[head_tail[4][1]] = head_tail[0][0];
        B[head_tail[0][1]] = head_tail[0][0];
        B[head_tail[1][1]] = head_tail[3][0];
        B[head_tail[2][1]] = head_tail[3][0];
        B[head_tail[3][1]] = head_tail[0][0];
        B[head_tail[4][1]] = head_tail[0][0];
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
    return a.calc_score() < b.calc_score();
}

int main()
{
    int n,l;
    cin >> n >> l;
    rep(i,0,N){
        cin >> T[i];
        T_rate[i] = (double)T[i]/L;
        if(T[i]>=5000){
            if(T[i]>=7500){
                grade[0].insert(i);
            }else{
                grade[1].insert(i);
            }
        }else{
            if(T[i]>=2500){
                grade[2].insert(i);
            }else if(T[i]>=100){
                grade[3].insert(i);
            }else{
                grade[4].insert(i);
            }
        }
    }
    Touban ans = Touban(0);
    ans.print_ans();
}