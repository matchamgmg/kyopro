#include<iostream>
#include<vector>
#include<set>
#include<queue>
#include<chrono>
#include<algorithm>
#include<numeric>
#include<map>
#include<atcoder/all>
using namespace std;
using namespace atcoder;

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define all(a) a.begin(),a.end()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

constexpr const int RAIL_COST = 100;
constexpr const int STATION_COST = 5000;
constexpr const int RAIL_PENALTY = 40;

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

    void printTime() const{
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
        auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;

        cout << duration_cast<milliseconds>(diff).count() << endl;
    }
};

struct Man{
    int is_;
    int js_;
    int it_;
    int jt_;
};

struct Coord{
    int x_;
    int y_;
};

bool operator<(const Coord &a, const Coord &b){
    return (a.x_ < b.x_ ? true : a.y_ < b.y_ ? true : false);
}

bool operator==(const Coord &a, const Coord &b){
    return (a.x_==b.x_ && a.y_==b.y_);
}

struct Rail{
    int x_;
    int y_;
    int num_;
};

struct Annealing{
    int score_;
    int turn_;
    int money_;
    int income_;
    vector<Man> &mans_;
    vector<int> &man_income_;
    vector<int> answer_;
    map<Coord,int> wa_check_;
    dsu uf;
    vector<vector<set<int>>> idx_map;

    Annealing(vector<int> &answer, vector<Man> &mans, vector<int> &man_income)
    : answer_(answer), mans_(mans), man_income_(man_income)
    {
        score_ = 0;
        turn_ = 0;
        money_ = 0;
        income_ = 0;
        uf = dsu(2500);
        idx_map = vector<vector<set<int>>>(50,vector<set<int>>(50,set<int>()));
    }

    void updateIncome(Coord &posi){
        rep(dx,-2,3)rep(dy,-2,3){
            if(abs(dx)+abs(dy)>2) continue;
            int x = posi.x_ + dx;
            int y = posi.y_ + dy;
            int z = 50*x + y;
            if(!grid_check(x,y,50,50)) continue;
            for(int i : idx_map[x][y]){
                int a = 50*mans_[i].is_ + mans_[i].js_;
                int b = 50*mans_[i].it_ + mans_[i].jt_;
                if(!uf.same(a,b)){
                    if(a==z){
                        swap(a,b);
                    }
                    
                }
            }
        }
    }

    bool Wait(){

    }

    bool putStation(vector<int> &posi){
        
    }

    bool putRail(vector<int> &posi){

    }

    void playGame(vector<string> &ans){
        vector<string> ans_omit_wait;
        rep(i,0,ans.size()){
            if(ans[i][0]!=-1){
                ans_omit_wait.emplace_back(ans[i]);
            }
        }
        rep(i,0,ans_omit_wait.size()){

        }
    }
};

int main(){

}