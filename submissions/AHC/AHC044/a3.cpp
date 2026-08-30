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
int T_ave[10];
vector<set<int>> grade(10);
map<int,int> i2g;

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
    int A[N],B[N];
    mt19937 mt;
    int head_tail[10][2];
    int now_score;

    public:
    Touban(const int seed) {
        mt = mt19937(seed);
        this->build_by_grade();
        now_score = 100000000;
    };

    public:
    void print_ans(){
        rep(i,0,N){
            cout << A[i] << ' ' << B[i] << '\n';
        }
    }

    void build_by_grade(){
        rep(i,0,10)rep(j,0,2) head_tail[i][j] = -1;
        rep(i,0,10){
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
        rep(i,0,10){
            A[head_tail[i][1]] = head_tail[(i+1)%10][0];
            B[head_tail[i][1]] = head_tail[(i+1)%10][0];
        }
        now_score = calc_score(A,B);
    }

    void rebuild(){
        auto mt_re = mt19937(mt());
        int A2[N],B2[N];
        rep(i,0,N){
            A2[i] = A[i];
            B2[i] = B[i];
        }
        rep(i,0,10){
            A2[head_tail[i][1]] = head_tail[mt_re()%10][0];
            B2[head_tail[i][1]] = head_tail[mt_re()%10][0];
        }
        int s = calc_score(A2,B2);
        if(now_score > s){
            rep(i,0,N){
                A[i] = A2[i];
                B[i] = B2[i];
            }
            now_score = s;
        }
    }

    int calc_score(int A_[], int B_[]){
        int S[10];
        int start = -1;
        rep(i,0,10){
            S[i] = 0;
        }
        start = i2g[0];
        S[start] = 1;
        int now = start;
        int next;
        rep(i,0,499){
            if(S[now]&1){
                next = A_[head_tail[now][1]];
                now = i2g[next];
                S[now]++;
            }else{
                next = B_[head_tail[now][1]];
                now = i2g[next];
                S[now]++;
            }
        }
        int e = 0;
        rep(i,0,10){
            e += abs((double)S[i]*1000-(double)T_ave[i]);
        }
        return e;
    }
};

int main()
{
    int n,l;
    cin >> n >> l;
    TimeKeeper tk(1900);
    rep(i,0,10){
        T_ave[i] = 0;
    }
    vector<int> idx(N);
    iota(all(idx),0);
    rep(i,0,N){
        cin >> T[i];
    }
    sort(all(idx),[&](int i, int j){ return T[i] < T[j]; });
    rep(i,0,N){
        grade[i/10].insert(idx[i]);
        T_ave[i/10] += T[idx[i]];
        i2g[idx[i]] = i/10;
    }
    Touban ans = Touban(0);
    while(!tk.isTimeOver()){
        ans.rebuild();
    }
    ans.print_ans();
}