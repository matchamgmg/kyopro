#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

vector<pair<int,int>> plant_seed(int n){
    vector<pair<int,int>> ret;
    // 21 20 19 18 17 36
    // 22  7  6  5 16 35
    // 23  8  1  4 15 34
    // 24  9  2  3 14 33
    // 25 10 11 12 13 32
    // 26 27 28 29 30 31
    vector<vector<bool>> grid(n,vector<bool>(n,false));
    int x = 2,y = 2;
    ret.push_back(make_pair(x,y));
    int dir = 0;
    grid[x][y] = true;
    vector<int> dx = {1,0,-1,0};
    vector<int> dy = {0,1,0,-1};
    rep(i,n*n-1){
        if(!grid[x+dx[(dir+1)%4]][y+dy[(dir+1)%4]]){
            dir = (dir+1) % 4;
        }
        x += dx[dir];
        y += dy[dir];
        grid[x][y] = true;

        ret.push_back(make_pair(x,y));
    }

    return ret;
}

vector<vector<int>> generate_grid(int n,vector<vector<int>> &x){
    vector<vector<int>> grid(n,vector<int>(n));

    int size_x = (int)x.size();
    vector<pair<int,int>> seed_list(size_x);
    rep(i,size_x){
        seed_list[i] = make_pair(accumulate(ALL(x[i]),0),i);
    }
    sort(ALL(seed_list),greater<pair<int,int>>{});

    vector<pair<int,int>> grid_idx = plant_seed(n);

    rep(i,n){
        rep(j,n){
            int a = grid_idx[i*n+j].first;
            int b = grid_idx[i*n+j].second;
            grid[a][b] = seed_list[i*n+j].second;
        }
    }
    return grid;
}

void print_grid(vector<vector<int>> &grid){
    rep(i,grid.size()){
        rep(j,grid[0].size()){
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    int N,M,T;
    cin >> N >> M >> T;
    vector<vector<int>> X(2*N*(N-1),vector<int>(M));
    rep(i,X.size()){
        rep(j,X[0].size()){
            cin >> X[i][j];
        }
    }

    rep(i,T){
        vector<vector<int>> grid = generate_grid(N,X);
        print_grid(grid);

        rep(i,X.size()){
            rep(j,X[0].size()){
                cin >> X[i][j];
            }
        }
    }
}