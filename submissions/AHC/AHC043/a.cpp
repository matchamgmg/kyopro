#include<iostream>
#include<vector>
using namespace std;

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define all(a) a.begin(),a.end()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

constexpr const int RAIL_COST = 100;
constexpr const int STATION_COST = 5000;

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

struct Rail{
    int x_;
    int y_;
    int num_;
};

class State{
    public:
    int N_,M_,K_,T_;
    int money_;
    int turn_;
    vector<Man> mans_;
    vector<int> remain_min_cost_;
    vector<vector<int>> field_;

    State(const int N, const int M, const int K, const int T)
    : N_(N), M_(M), K_(K), T_(T) {
        turn_ = 0;
        money_  = K_;
        mans_.assign(M,{0,0,0,0});
        remain_min_cost_.assign(M,0);
        rep(i,0,M_){
            cin >> this->mans_[i].is_ >> this->mans_[i].js_ >> this->mans_[i].it_ >> this->mans_[i].jt_;
            this->remain_min_cost_[i] = RAIL_COST*(abs(this->mans_[i].is_-this->mans_[i].it_) + abs(this->mans_[i].js_-this->mans_[i].jt_) - 5) + STATION_COST*2;
        }
        this->field_.assign(N_,vector<int>(N_,-1));
    }

    public:
    void isBudgetOut(){}
    bool isTurnOver(){
        return this->turn_ >= T_;
    }
    bool Wait(){
        cout << -1 << '\n';
        if(!runPhase()) return false;
        return true;
    }

    //to do
    Rail nextRailBeam(const Coord &start, const Coord &goal){
        vector<int> dx = {0,1,0,-1}, dy = {1,0,-1,0};
        Coord now_posi = start;
        Rail res;
        int prev_i;
        rep(t,0,2){
            int next_min_dist = 1<<29;
            rep(i,0,4){
                int nx = now_posi.x_ + dx[i];
                int ny = now_posi.y_ + dy[i];
                if(!grid_check(nx,ny,N_,N_)) continue;
                int ndist = abs(nx-goal.x_) + abs(ny-goal.y_);
                if(ndist < next_min_dist){
                    next_min_dist = ndist;
                    if(t==0){
                        res.x_ = nx;
                        res.y_ = ny;
                        prev_i = i;
                    }else{
                        if((prev_i+i)%2==0){
                            if(i%2==0){
                                res.num_ = 1;
                            }else{
                                res.num_ = 2;
                            }
                        }else if((prev_i+i)%4==1){
                            if(i==1 || i==2){
                                res.num_ = 3;
                            }else{
                                res.num_ = 5;
                            }
                        }else{
                            if(i==2 || i==3){
                                res.num_ = 4;
                            }else{
                                res.num_ = 6;
                            }
                        }
                    }
                }
            }
            now_posi.x_ = res.x_;
            now_posi.y_ = res.y_;
        }
        return res;
    }
    bool structRail(const Coord &start, const Coord &goal){
        Coord now_posi = start;
        while(!(abs(now_posi.x_-goal.x_) + abs(now_posi.y_-goal.y_)<=1)){
            Rail next_rail = nextRailBeam(now_posi,goal);
            while(this->money_ < RAIL_COST){
                if(!Wait()) return false;
            }
            cout << next_rail.num_ << ' ' << next_rail.x_ << ' ' << next_rail.y_ << '\n';
            now_posi.x_ = next_rail.x_;
            now_posi.y_ = next_rail.y_;
            if(!runPhase()) return false;
        }
        return true;
    }
    bool structStation(const Coord &posi){
        while(this->money_ < STATION_COST){
            if(!Wait()) return false;
        }
        this->field_[posi.x_][posi.y_] = 0;
        cout << 0 << ' ' << posi.x_ << ' ' << posi.y_ << '\n';
        if(!runPhase()) return false;
        return true;
    }
    void structNone(){}
    void structPhase(){}
    bool runPhase(){
        this->turn_++;
        if(isTurnOver()) return false;
        return true;
    }
    void calcIncome(){}
    int firstEvaluate(const Coord &posi1, const Coord &posi2){
        int cost = RAIL_COST*(abs(posi1.x_-posi2.x_) + abs(posi1.y_-posi2.y_) - 1);
        if(cost > money_-10000) return -1;
        int cnt = 0;
        int dist = 0;
        for(Man &man : this->mans_){
            dist = abs(man.is_-posi1.x_) + abs(man.js_-posi1.y_);
            if(dist <= 2) cnt++;
            dist = abs(man.it_-posi2.x_) + abs(man.jt_-posi2.y_);
            if(dist <= 2) cnt++;
        }
        return cnt*100 + cost;
    }
    bool firstAction(){
        vector<int> candidates;
        rep(i,0,M_){
            if(this->remain_min_cost_[i] <= money_){
                candidates.emplace_back(i);
            }
        }
        int first_best_score = -1;
        Coord best_position1 = {0,0};
        Coord best_position2 = {0,0};
        for(int i : candidates){
            int score;
            Coord posi1;
            Coord posi2;
            rep(dx1,-2,3)rep(dy1,-2,3){
                if(abs(dx1)+abs(dy1)>2) continue;
                posi1.x_ = mans_[i].is_ + dx1;
                posi1.y_ = mans_[i].js_ + dy1;
                if(!grid_check(posi1.x_,posi1.y_,N_,N_)) continue;
                rep(dx2,-2,3)rep(dy2,-2,3){
                    if(abs(dx2)+abs(dy2)>2) continue;
                    posi2.x_ = mans_[i].it_ + dx2;
                    posi2.y_ = mans_[i].jt_ + dy2;
                    if(!grid_check(posi2.x_,posi2.y_,N_,N_)) continue;
                    score = firstEvaluate(posi1,posi2);
                    if(first_best_score < score){
                        first_best_score = score;
                        best_position1 = posi1;
                        best_position2 = posi2;
                    }
                }
            }
        }
        // 決定した場所に駅を設置する
        if(!structStation(best_position1)) return false;
        if(!structStation(best_position2)) return false;
        // 駅の間に線路を設置する
        if(!structRail(best_position1,best_position2)) return false;
        return true;
    }

    bool Fill(){
        while(Wait()){

        }
        return true;
    }
};

void playGame(State &state){
    state.firstAction();
    state.Fill();
}

int main(){
    // N = 50
    // 50 <= M <= 1600
    // 11000 <= K <= 20000
    // T = 800
    // 家と職場の距離 > 4
    int N,M,K,T;
    cin >> N >> M >> K >> T;
    State state(N,M,K,T);
    playGame(state);
}