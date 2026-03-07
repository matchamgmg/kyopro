#include<iostream>
#include<vector>
#include<set>
#include<queue>
#include<chrono>
#include<algorithm>
#include<numeric>
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

class State{
    public:
    int N_,M_,K_,T_;
    int money_;
    int turn_;
    int income_;
    vector<Man> mans_;
    vector<int> man_income_;
    vector<int> man_idx_;
    vector<vector<int>> field_, house_field_;
    vector<vector<bool>> station_covered_;
    set<pair<int,int>> half_covered_;
    set<int> full_covered_;
    set<Coord> stations_;

    State(const int N, const int M, const int K, const int T)
    : N_(N), M_(M), K_(K), T_(T) {
        income_ = 0;
        turn_ = 0;
        money_ = K_;
        this->mans_.assign(M,{0,0,0,0});
        this->man_income_.assign(M,0);
        this->man_idx_.assign(M,0);
        this->field_.assign(N_,vector<int>(N_,-1));
        this->house_field_.assign(N_,vector<int>(N_,0));
        this->station_covered_.assign(N_,vector<bool>(N_,false));
        rep(i,0,M_){
            cin >> this->mans_[i].is_ >> this->mans_[i].js_ >> this->mans_[i].it_ >> this->mans_[i].jt_;
            this->man_income_[i] = abs(mans_[i].is_-mans_[i].it_) + abs(mans_[i].js_-mans_[i].jt_);
            this->house_field_[this->mans_[i].is_][this->mans_[i].js_]++;
            this->house_field_[this->mans_[i].it_][this->mans_[i].jt_]++;
        }
        iota(all(man_idx_),0);
    }

    public:
    bool isTurnOver(){
        return this->turn_ >= T_;
    }
    bool Wait(){
        cout << -1 << '\n';
        if(!runPhase()) return false;
        return true;
    }

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
        queue<Coord> station_que;
        while(!(abs(now_posi.x_-goal.x_) + abs(now_posi.y_-goal.y_)<=1)){
            Rail next_rail = nextRailBeam(now_posi,goal);
            int &num = this->field_[next_rail.x_][next_rail.y_];
            if(num>=0){
                if(num==0 || num==next_rail.num_){
                    now_posi.x_ = next_rail.x_;
                    now_posi.y_ = next_rail.y_;
                    continue;
                }else{
                    now_posi.x_ = next_rail.x_;
                    now_posi.y_ = next_rail.y_;
                    station_que.push(now_posi);
                    continue;
                }
            }
            while(this->money_ < RAIL_COST){
                if(!Wait()) return false;
            }
            cout << next_rail.num_ << ' ' << next_rail.x_ << ' ' << next_rail.y_ << '\n';
            num = next_rail.num_;
            now_posi.x_ = next_rail.x_;
            now_posi.y_ = next_rail.y_;
            this->money_ -= RAIL_COST;
            if(!runPhase()) return false;
        }
        while(!station_que.empty()){
            Coord posi = station_que.front();
            station_que.pop();
            if(!structStation(posi)) return false;
        }
        return true;
    }

    void insertMan(const int a){
        if(this->half_covered_.count({man_income_[a],a})){
            this->half_covered_.erase({man_income_[a],a});
            this->full_covered_.insert(a);
        }else if(this->full_covered_.count(a)){

        }else{
            this->half_covered_.insert({man_income_[a],a});
        }
    }

    void insertStation(const Coord station){
        this->stations_.insert(station);
    }

    void updateStationCovered(const Coord &station){
        rep(dx,-2,3)rep(dy,-2,3){
            if(abs(dx)+abs(dy)>2) continue;
            int x = station.x_ + dx;
            int y = station.y_ + dy;
            if(!grid_check(x,y,N_,N_)) continue;
            this->station_covered_[x][y] = true;
        }
    }

    void updateIncome(){
        for(int i : this->full_covered_){
            this->income_ += abs(mans_[i].is_-mans_[i].it_) + abs(mans_[i].js_-mans_[i].jt_);
        }
        this->full_covered_.clear();
    }

    void updateHouseField(const Coord &posi){
        rep(dx,-2,3)rep(dy,-2,3){
            if(abs(dx)+abs(dy)>2) continue;
            int x = posi.x_ + dx;
            int y = posi.y_ + dy;
            if(!grid_check(x,y,N_,N_)) continue;
            this->house_field_[x][y] = 0;
        }
    }

    bool structStation(const Coord &posi){
        while(this->money_ < STATION_COST){
            if(!Wait()) return false;
        }
        this->field_[posi.x_][posi.y_] = 0;
        updateStationCovered(posi);
        rep(i,0,M_){
            Man man = this->mans_[i];
            if((this->station_covered_[man.is_][man.js_] && this->house_field_[man.is_][man.js_]>0) || (this->station_covered_[man.it_][man.jt_] && this->house_field_[man.it_][man.jt_]>0)){
                insertMan(i);
            }
        }
        updateHouseField(posi);
        cout << 0 << ' ' << posi.x_ << ' ' << posi.y_ << '\n';
        insertStation(posi);
        this->money_ -= STATION_COST;
        if(!runPhase()) return false;
        return true;
    }
    bool runPhase(){
        this->money_ += this->income_;
        this->turn_++;
        if(isTurnOver()) return false;
        return true;
    }
    void calcIncome(){}
    int firstEvaluate(const Coord &posi1, const Coord &posi2){
        int cost = RAIL_COST*(abs(posi1.x_-posi2.x_) + abs(posi1.y_-posi2.y_) - 1);
        if(cost > money_-10000) return -1;
        int cnt = 0;
        int dist1 = 0;
        int dist2 = 0;
        int income = 0;
        for(Man &man : this->mans_){
            dist1 = abs(man.is_-posi1.x_) + abs(man.js_-posi1.y_);
            if(dist1 <= 2) cnt++;
            dist2 = abs(man.it_-posi2.x_) + abs(man.jt_-posi2.y_);
            if(dist2 <= 2) cnt++;
            if(dist1<=2 && dist2<=2){
                income += abs(man.is_-man.it_) + abs(man.js_-man.jt_);
            }
        }
        return income*100 + cnt*10 - cost;
    }
    bool firstAction(){
        vector<int> candidates;
        rep(i,0,M_){
            if(this->man_income_[i] <= (money_ - 2*STATION_COST + 5*RAIL_COST)/100){
                candidates.emplace_back(i);
            }
        }
        sort(all(candidates),[&](int i, int j){ return man_income_[i] > man_income_[j]; });
        int first_best_score = -1;
        Coord best_position1 = {0,0};
        Coord best_position2 = {0,0};
        rep(i,0,min((int)candidates.size(),100)){
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
        updateIncome();
        return true;
    }

    Coord searchDestination(const Man &man){
        Coord posi;
        posi.x_ = man.is_;
        posi.y_ = man.js_;
        if(this->station_covered_[posi.x_][posi.y_]==true){
            posi.x_ = man.it_;
            posi.y_ = man.jt_;
        }
        return posi;
    }

    int secondEvaluate(Coord &station_posi, int &new_income){
        int income = 0;
        int cnt = 0;
        for(auto p : half_covered_){
            int i = p.second;
            Man man = this->mans_[i];
            Coord man_posi1, man_posi2;
            man_posi1.x_ = man.is_;
            man_posi1.y_ = man.js_;
            man_posi2.x_ = man.it_;
            man_posi2.y_ = man.jt_;
            if(station_covered_[man_posi2.x_][man_posi2.y_]==true){
                swap(man_posi1,man_posi2);
            }
            int dist = abs(man_posi2.x_-station_posi.x_) + abs(man_posi2.y_-station_posi.y_);
            if(dist<=2){
                income += abs(man.is_-man.it_) + abs(man.js_-man.jt_);
            }
        }
        rep(dx,-2,3)rep(dy,-2,3){
            if(abs(dx)+abs(dy)>2) continue;
            int x = station_posi.x_ + dx;
            int y = station_posi.y_ + dy;
            if(!grid_check(x,y,N_,N_)) continue;
            cnt += this->house_field_[x][y];
        }
        new_income = income;
        return income*10 + cnt;
    }

    double evaluateRequireTurn(const Coord &goal, vector<Rail> &railway){
        railway.clear();
        
        Coord nearest_rail;
        int nearest_rail_dist = 1<<29;
        Coord start = *this->stations_.begin();
        priority_queue<pair<int,Coord>,vector<pair<int,Coord>>,greater<pair<int,Coord>>> st;
        queue<Coord> que;
        vector<int> dx = {0,1,0,-1}, dy = {1,0,-1,0};
        vector<vector<bool>> seen(N_,vector<bool>(N_,false));
        vector<vector<int>> dist(N_,vector<int>(N_,(1<<29)));

        // 始点を探索
        que.push(start);
        st.push({abs(start.x_-goal.x_)+abs(start.y_-goal.y_),start});
        seen[start.x_][start.y_] = true;
        Coord now_posi;
        while(!que.empty()){
            now_posi = que.front();
            que.pop();
            rep(i,0,4){
                int x = now_posi.x_ + dx[i];
                int y = now_posi.y_ + dy[i];
                if(!grid_check(x,y,N_,N_)) continue;
                if(this->field_[x][y]<0) continue;
                if(seen[x][y]==true) continue;
                int ndist = abs(x-goal.x_) + abs(y-goal.y_);
                if(this->field_[x][y]>0){
                    if(ndist < nearest_rail_dist){
                        nearest_rail_dist = ndist;
                        nearest_rail.x_ = x;
                        nearest_rail.y_ = y;
                    }
                }else{
                    st.push({ndist,{x,y}});
                    dist[x][y] = 0;
                }
                seen[x][y] = true;
                que.push({x,y});
            }
        }

        //nearest_rail -> st
        start = nearest_rail;
        now_posi = start;
        dist[now_posi.x_][now_posi.y_] = 0 + RAIL_PENALTY;
        while(!(now_posi==goal)){
            int min_ndist = 1<<29;
            Coord next_posi = {-1,-1};
            rep(i,0,4){
                int x = now_posi.x_ + dx[i];
                int y = now_posi.y_ + dy[i];
                if(!grid_check(x,y,N_,N_)) continue;
                if(dist[x][y]<(1<<29)) continue;
                if(this->field_[x][y]>=0) continue;
                int ndist = abs(x-goal.x_) + abs(y-goal.y_);
                if(min_ndist > ndist){
                    min_ndist = ndist;
                    dist[x][y] = dist[now_posi.x_][now_posi.y_] + 1;
                    next_posi.x_ = x;
                    next_posi.y_ = y;
                }
            }
            if(next_posi.x_==-1 && next_posi.y_==-1) break;
            now_posi = next_posi;
        }

        while(!st.empty()){
            now_posi = st.top().second;
            st.pop();
            Coord prev_posi = {-1,-1};
            bool is_valid = true;
            while(!(now_posi==goal)){
                int min_ndist = 1<<29;
                Coord next_posi = {-1,-1};
                rep(i,0,4){
                    int x = now_posi.x_ + dx[i];
                    int y = now_posi.y_ + dy[i];
                    if(!grid_check(x,y,N_,N_)) continue;
                    if(this->field_[x][y]>0) continue;
                    if(x==prev_posi.x_ && y==prev_posi.y_) continue;
                    int ndist = abs(x-goal.x_) + abs(y-goal.y_);
                    if(min_ndist > ndist){
                        if(dist[x][y] <= dist[now_posi.x_][now_posi.y_]+1){
                            is_valid = false;
                            break;
                        }
                        min_ndist = ndist;
                        dist[x][y] = dist[now_posi.x_][now_posi.y_] + 1;
                        next_posi.x_ = x;
                        next_posi.y_ = y;
                    }
                }
                if(next_posi.x_==-1 && next_posi.y_==-1) is_valid = false;
                else if(this->field_[next_posi.x_][next_posi.y_]==0) is_valid = false;
                else if(dist[next_posi.x_][next_posi.y_]>dist[goal.x_][goal.y_]) is_valid = false;
                else if(dist[next_posi.x_][next_posi.y_]>100) is_valid = false;
                if(!is_valid) break;
                prev_posi = now_posi;
                now_posi = next_posi;
            }
        }

            // rep(i,0,N_){
            //     rep(j,0,N_){
            //         if(dist[i][j]==(1<<29)){
            //             printf("  .");
            //         }else{
            //             printf("%3d",dist[i][j]);
            //         }
            //     }
            //     cout << endl;
            // }

        // find railway
        now_posi = goal;
        while(!(dist[now_posi.x_][now_posi.y_]==0 || now_posi==nearest_rail)){
            Rail next_rail = {-1,-1,-1};
            int prev_i = -1;
            rep(t,0,2){
                if(t==1){
                    now_posi.x_ = next_rail.x_;
                    now_posi.y_ = next_rail.y_;
                }
                rep(i,0,4){
                    int x = now_posi.x_ + dx[i];
                    int y = now_posi.y_ + dy[i];
                    if(!grid_check(x,y,N_,N_)) continue;
                    if(dist[x][y]!=dist[now_posi.x_][now_posi.y_]-1) continue;
                    if(t==0){
                        next_rail.x_ = x;
                        next_rail.y_ = y;
                        prev_i = i;
                    }else{
                        if((prev_i+i)%2==0){
                            if(i%2==0){
                                next_rail.num_ = 1;
                            }else{
                                next_rail.num_ = 2;
                            }
                        }else if((prev_i+i)%4==1){
                            if(i==1 || i==2){
                                next_rail.num_ = 3;
                            }else{
                                next_rail.num_ = 5;
                            }
                        }else{
                            if(i==2 || i==3){
                                next_rail.num_ = 4;
                            }else{
                                next_rail.num_ = 6;
                            }
                        }
                    }
                }
            }
            if(next_rail.x_==-1 && next_rail.y_==-1) return -10000;
            if(next_rail.x_==nearest_rail.x_ && next_rail.y_==nearest_rail.y_) railway.push_back({next_rail.x_,next_rail.y_,0});
            else if(!this->field_[next_rail.x_][next_rail.y_]==0) railway.push_back(next_rail);
        }
        railway.push_back({goal.x_,goal.y_,0});
        // rep(i,0,N_){
        //     rep(j,0,N_){
        //         if(dist[i][j]==(1<<29)){
        //             printf("  .");
        //         }else{
        //             printf("%3d",dist[i][j]);
        //         }
        //     }
        //     cout << endl;
        // }

        int rail_cnt = 0;
        int station_cnt = 0;
        rep(i,0,railway.size()){
            if(railway[i].num_==0){
                station_cnt++;
            }else{
                rail_cnt++;
            }
        }
        int cost = station_cnt*STATION_COST + rail_cnt*RAIL_COST;
        int now_income = this->income_;
        return (double)cost / now_income;
    }

    bool structRailway(const vector<Rail> &railway){
        queue<Rail> que;
        for(Rail rail : railway){
            if(rail.num_==0){
                que.push(rail);
            }else{
                while(RAIL_COST > this->money_){
                    if(!Wait()) return false;
                }
                cout << rail.num_ << ' ' << rail.x_ << ' ' << rail.y_ << '\n';
                this->field_[rail.x_][rail.y_] = rail.num_;
                this->money_ -= RAIL_COST;
                if(!runPhase()) return false;
            }
        }
        while(!que.empty()){
            Coord rail;
            rail.x_ = que.front().x_;
            rail.y_ = que.front().y_;
            que.pop();
            if(!structStation(rail)) return false;
        }
        return true;
    }

    bool doSecondAction(const vector<Rail> &railway, int new_income){
        int now_money = this->money_;
        int now_income = this->income_;
        int now_turn = this->turn_;
        int turn = 0;
        queue<Rail> que;
        for(Rail rail : railway){
            if(rail.num_==0){
                que.push(rail);
            }else{
                while(RAIL_COST > now_money){
                    now_money += now_income;
                    turn++;
                }
                now_money -= RAIL_COST;
                now_money += now_income;
                turn++;
            }
        }
        while(!que.empty()){
            Coord rail;
            rail.x_ = que.front().x_;
            rail.y_ = que.front().y_;
            que.pop();
            while(STATION_COST > now_money){
                now_money += now_income;
                turn++;
            }
            now_money -= STATION_COST;
            now_money += now_income;
        }
        if(now_turn+turn > T_) return false;
        int final_money1 = (T_-now_turn+1)*now_income + this->money_;
        int final_money2 = now_money - now_income + (T_-now_turn-turn+1)*(now_income+new_income);
        return final_money1 <= final_money2;
    }

    bool secondAction(){
        TimeKeeper time_keeper(50);
        double best_score = -1;
        int best_new_income = -1;
        Coord best_posi = {-1,-1};
        vector<Rail> best_railway;
        for(auto p : this->half_covered_){
            if(time_keeper.isTimeOver()) break;
            int i = p.second;
            Coord destination = searchDestination(mans_[i]);
            Coord next_posi;
            rep(dx,-2,3)rep(dy,-2,3){
                if(abs(dx)+abs(dy)>2) continue;
                next_posi.x_ = destination.x_+dx;
                next_posi.y_ = destination.y_+dy;
                if(!grid_check(next_posi.x_,next_posi.y_,N_,N_)) continue;
                int new_income;
                int score1 = secondEvaluate(next_posi,new_income);
                vector<Rail> railway;
                double require_turn = evaluateRequireTurn(next_posi, railway);
                if(require_turn + this->turn_ >= T_) continue;
                double score2 = (double)score1 / require_turn;
                if(best_score < score2){
                    best_score = score2;
                    best_posi = next_posi;
                    best_railway = railway;
                    best_new_income = new_income;
                }
            }
        }
        if(best_score<0) return false;
        if(!doSecondAction(best_railway,best_new_income)) return false;
        if(!structRailway(best_railway)) return false;
        return true;
    }

    bool Fill(){
        while(Wait()){

        }
        return true;
    }
};

void playGame(State &state){
    TimeKeeper tk = TimeKeeper(2500);
    state.firstAction();
    // tk.printTime();
    while(!tk.isTimeOver()){
        if(!state.secondAction()) break;
    }
    state.Fill();
    // tk.printTime();
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