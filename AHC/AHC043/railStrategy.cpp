#include<iostream>
#include<vector>
#include<set>
#include<queue>
#include<chrono>
using namespace std;

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define all(a) a.begin(),a.end()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

struct Coord{
    int x_;
    int y_;
};

struct Rail{
    int x_;
    int y_;
    int num_;
};

bool operator<(const Coord &a, const Coord &b){
    return (a.x_ < b.x_ ? true : a.y_ < b.y_ ? true : false);
}

bool operator==(const Coord &a, const Coord &b){
    return (a.x_==b.x_ && a.y_==b.y_);
}

int main(){
    int N = 30;
    vector<vector<int>> field(N,vector<int>(N,-1));
    set<Coord> stations;
    field[3][3] = 0;
    field[20][13] = 0;
    field[25][10] = 9;
    stations.insert({3,3});
    stations.insert({20,13});
    rep(i,4,13){
        field[3][i] = 1;
    }
    field[3][13] = 3;
    rep(j,4,20){
        field[j][13] = 2;
    }
    rep(i,0,N){
        rep(j,0,N){
            if(field[i][j]==-1) printf("  .");
            else printf("%3d",field[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // [i][j]に新たに駅を追加するとき
    // その駅に対してどこから線路を伸ばすかを決める
    // どのようにして線路を伸ばすかを決める

    // 既存駅から線路を伸ばすとき、既存の線路は壁として考える
    // 既存の線路から線路を伸ばすとき、一番近い線路からは必ずたどり着ける
    // マンハッタン距離に関して貪欲に探索する
    // 線路はすべて連結であると仮定

    Coord nearest_rail;
    int nearest_rail_dist = 1<<29;
    priority_queue<pair<int,Coord>,vector<pair<int,Coord>>,greater<pair<int,Coord>>> st;
    Coord start = *stations.begin();
    Coord goal;
    goal.x_ = 25;
    goal.y_ = 10;
    st.push({abs(start.x_-goal.x_)+abs(start.y_-goal.y_),start});
    queue<Coord> que;
    vector<int> dx = {0,1,0,-1}, dy = {1,0,-1,0};
    vector<vector<bool>> seen(N,vector<bool>(N,false));
    que.push(start);
    st.push({abs(start.x_-goal.x_)+abs(start.y_-goal.y_),start});
    seen[start.x_][start.y_] = true;
    while(!que.empty()){
        Coord now_posi = que.front();
        que.pop();
        rep(i,0,4){
            int x = now_posi.x_ + dx[i];
            int y = now_posi.y_ + dy[i];
            if(!grid_check(x,y,N,N)) continue;
            if(field[x][y]<0) continue;
            if(seen[x][y]==true) continue;
            int ndist = abs(x-goal.x_) + abs(y-goal.y_);
            if(field[x][y]>0){
                if(ndist < nearest_rail_dist){
                    nearest_rail_dist = ndist;
                    nearest_rail.x_ = x;
                    nearest_rail.y_ = y;
                }
            }else{
                st.push({ndist,{x,y}});
            }
            seen[x][y] = true;
            que.push({x,y});
        }
    }

    // cout << nearest_rail.x_ << ',' << nearest_rail.y_ << ':' << nearest_rail_dist << endl;
    // while(!st.empty()){
    //     Coord s = st.top().second;
    //     int d = st.top().first;
    //     st.pop();
    //     cout << s.x_ << ',' << s.y_ << ':' << d << endl;

    // }

    // ここから経路探索
    // rail -> stationの順
    vector<vector<int>> dist(N,vector<int>(N,(1<<29)));
    const int RAIL_PENALTY = 40;
    start = nearest_rail;
    Coord now_posi = start;
    dist[now_posi.x_][now_posi.y_] = 0 + RAIL_PENALTY;
    while(!(now_posi==goal)){
        int min_ndist = 1<<29;
        Coord next_posi = {-1,-1};
        rep(i,0,4){
            int x = now_posi.x_ + dx[i];
            int y = now_posi.y_ + dy[i];
            if(!grid_check(x,y,N,N)) continue;
            if(dist[x][y]<(1<<29)) continue;
            if(field[x][y]>=0 && field[x][y]<9) continue;
            int ndist = abs(x-goal.x_) + abs(y-goal.y_);
            if(min_ndist > ndist){
                min_ndist = ndist;
                dist[x][y] = dist[now_posi.x_][now_posi.y_] + 1;
                next_posi.x_ = x;
                next_posi.y_ = y;
            }
        }
        now_posi = next_posi;
    }
    rep(i,0,N){
        rep(j,0,N){
            if(dist[i][j]==(1<<29)) printf("  .");
            else printf("%3d",dist[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    while(!st.empty()){
        now_posi = st.top().second;
        if(dist[now_posi.x_][now_posi.y_]==(1<<29)) dist[now_posi.x_][now_posi.y_] = 0;
        st.pop();
        Coord prev_posi = {-1,-1};
        bool is_valid = true;
        while(!(now_posi==goal)){
            int min_ndist = 1<<29;
            Coord next_posi = {-1,-1};
            rep(i,0,4){
                int x = now_posi.x_ + dx[i];
                int y = now_posi.y_ + dy[i];
                if(!grid_check(x,y,N,N)) continue;
                if(field[x][y]>0 && field[x][y]<9) continue;
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
            else if(dist[next_posi.x_][next_posi.y_]>dist[goal.x_][goal.y_]) is_valid = false;
            else if(dist[next_posi.x_][next_posi.y_]>100) is_valid = false;
            if(!is_valid) break;
            prev_posi = now_posi;
            now_posi = next_posi;
        }
    }

    rep(i,0,N){
        rep(j,0,N){
            if(dist[i][j]==(1<<29)) printf("  .");
            else printf("%3d",dist[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    //線路構築
    //終了条件 dist[i][j] == 0 or (i,j) == nearest_rail
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
                if(!grid_check(x,y,N,N)) continue;
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
        if(next_rail.x_==nearest_rail.x_ && next_rail.y_==nearest_rail.y_) field[next_rail.x_][next_rail.y_] = 0;
        else if(!field[next_rail.x_][next_rail.y_]==0) field[next_rail.x_][next_rail.y_] = next_rail.num_;
    }
    field[goal.x_][goal.y_] = 0;

    rep(i,0,N){
        rep(j,0,N){
            if(field[i][j]==-1) printf("  .");
            else printf("%3d",field[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}