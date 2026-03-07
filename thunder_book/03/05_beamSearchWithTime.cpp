#include<sstream>
#include<iostream>
#include<vector>
#include<random>
#include<queue>
#include<chrono>

int cnt = 0;

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

struct Coord{
    int y_;
    int x_;
    Coord(const int y = 0, const int x = 0) : y_(y), x_(x) {}
};

constexpr const int H = 30;
constexpr const int W = 30;
constexpr int END_TURN = 100;

using ScoreType = int64_t;
constexpr const ScoreType INF = 1000000000LL;

class MazeState{
    private:
    int points_[H][W] = {};
    int turn_ = 0;

    public:
    Coord character_ = Coord();
    int game_score_ = 0;
    MazeState() {}

    MazeState(const int seed){
        auto mt_for_construct = std::mt19937(seed);
        this->character_.y_ = mt_for_construct() % H;
        this->character_.x_ = mt_for_construct() % W;

        for(int y = 0; y < H; y++)
        for(int x = 0; x < W; x++){
            if(y == character_.y_ && x == character_.x_){
                continue;
            }
            this->points_[y][x] = mt_for_construct() % 10;
        }
    }

    public:
    bool isDone() const{
        return this->turn_ == END_TURN;
    }

    private:
    static constexpr const int dx[4] = {1, -1, 0, 0};
    static constexpr const int dy[4] = {0, 0, 1, -1};

    public:
    void advance(const int action){
        this->character_.x_ += dx[action];
        this->character_.y_ += dy[action];
        auto &point = this->points_[this->character_.y_][this->character_.x_];
        if(point > 0){
            this->game_score_ += point;
            point = 0;
        }
        this->turn_++;
    }

    std::vector<int> legalActions() const{
        std::vector<int> actions;
        for(int action = 0; action < 4; action++){
            int ty = this->character_.y_ + dy[action];
            int tx = this->character_.x_ + dx[action];
            if(ty >= 0 && ty < H && tx >= 0 && tx < W){
                actions.emplace_back(action);
            }
        }
        return actions;
    }

    public:
    std::string toString() const{
        std::stringstream ss;
        ss << "turn:\t" << this->turn_ << "\n";
        ss << "score:\t" << this->game_score_ << "\n";
        for(int h = 0; h < H; h++){
            for(int w = 0; w < W; w++){
                if(this->character_.y_ == h && this->character_.x_ == w){
                    ss << '@';
                }else if(this->points_[h][w] > 0){
                    ss << points_[h][w];
                }else{
                    ss << '.';
                }
            }
            ss << '\n';
        }
        return ss.str();
    }

    public:
    ScoreType evaluated_score_ = 0;
    void evaluateScore(){
        this->evaluated_score_ = this->game_score_;
    }

    public:
    int first_action_ = -1;
};

bool operator<(const MazeState &maze_1, const MazeState &maze_2){
    return maze_1.evaluated_score_ < maze_2.evaluated_score_;
}

using State = MazeState;

int beamSearchActionWithTimeThreshold(const State &state, const int beam_width, const int64_t time_threshold){
    auto time_keeper = TimeKeeper(time_threshold);
    std::priority_queue<State> now_beam;
    State best_state;

    now_beam.push(state);
    for(int t = 0;; t++){
        std::priority_queue<State> next_beam;
        for(int i = 0; i < beam_width; i++){
            if(time_keeper.isTimeOver()){
                cnt++;
                return best_state.first_action_;
            }
            if(now_beam.empty()) break;
            State now_state = now_beam.top();
            now_beam.pop();
            auto legal_actions = now_state.legalActions();
            for(const auto &action : legal_actions){
                State next_state = now_state;
                next_state.advance(action);
                next_state.evaluateScore();
                if(t==0) next_state.first_action_ = action;
                next_beam.push(next_state);
            }
        }

        now_beam = next_beam;
        best_state = now_beam.top();

        if(best_state.isDone()) break;
    }
    return best_state.first_action_;
}

void testAiScore(const int game_number){
    using std::cout;
    using std::endl;
    std::mt19937 mt_for_construct(0);
    double score_mean = 0;
    for(int i = 0; i < game_number; i++){
        auto state = State(mt_for_construct());

        while(!state.isDone()){
            state.advance(beamSearchActionWithTimeThreshold(state, 5, 10));
        }
        auto score = state.game_score_;
        score_mean += score;
    }
    score_mean /= (double)game_number;
    cout << "Score:\t" << score_mean << endl;
}

int main(){
    testAiScore(100);
    std::cout << cnt << std::endl;
    return 0;
}