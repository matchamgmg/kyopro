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

void fprint_ans(vector<pair<int,int>> &ans){
    ofstream ofs("0000.txt");
    int len = ans.size();
    rep(i,0,len){
        ofs << ans[i].first << ' ' << ans[i].second << '\n';
    }
}

void print_ans(vector<pair<int,int>> &ans){
    int len = ans.size();
    rep(i,0,len){
        cout << ans[i].first << ' ' << ans[i].second << '\n';
    }
}

int main()
{
    int N,M;
    cin >> N >> M;
    vector<string> S(N);
    set<pair<int,int>> Rock;
    rep(i,0,N){
        cin >> S[i];
        rep(j,0,N){
            if(S[i][j] == '#') Rock.insert({i,j});
        }
    }
    int len = N*N-M;
    vector<pair<int,int>> P(len);
    vector<int> dx = {0,1,0,-1}, dy = {1,0,-1,0};

    vector<vector<double>> probability(N,vector<double>(N,0));
    const int inf = 1e9;

    rep(i,0,N){
        rep(j,0,N){
            if(S[i][j] == '#'){
                probability[i][j] = inf;
                continue;
            }
            rep(d,0,4){
                int x = i;
                int y = j;
                while(grid_check(x+dx[d],y+dy[d],N,N) && S[x+dx[d]][y+dy[d]] != '#'){
                    x += dx[d];
                    y += dy[d];
                }
                probability[x][y] += (0.25/len);
            }
        }
    }

    vector<vector<double>> next_probability(N,vector<double>(N,0));

    rep(l,0,len){
        int best_x = -1, best_y = -1;
        double min_prob = 5000;
        double dbg = 0;
        rep(i,0,N){
            rep(j,0,N){
                // if(probability[i][j] < inf-1){
                //     dbg += probability[i][j];
                // }
                if(probability[i][j] < min_prob){
                    min_prob = probability[i][j];
                    best_x = i;
                    best_y = j;
                }
            }
        }
        // cout << dbg << '\n';
        P[l] = {best_x,best_y};
        // 再計算
        S[best_x][best_y] = '#';

        rep(i,0,N){
            rep(j,0,N){
                next_probability[i][j] = 0;
            }
        }

        rep(i,0,N){
            rep(j,0,N){
                if(S[i][j] == '#'){
                    next_probability[i][j] = inf;
                    continue;
                }
                rep(d,0,4){
                    int x = i;
                    int y = j;
                    while(grid_check(x+dx[d],y+dy[d],N,N) && S[x+dx[d]][y+dy[d]] != '#'){
                        x += dx[d];
                        y += dy[d];
                    }
                    next_probability[x][y] += probability[i][j]/4; //0除算回避
                }
            }
        }

        probability = next_probability;
    }
    print_ans(P);
}