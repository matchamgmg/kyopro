#include<bits/stdc++.h>
using namespace std;

int main(){
    int H, W;
    cin >> H >> W;
    vector A(H, vector<int>(W)), B(H, vector<int>(W));
    for(int i=0; i<H; ++i){
        for(int j=0; j<W; ++j) cin >> A[i][j];
    }
    for(int i=0; i<H; ++i){
        for(int j=0; j<W; ++j) cin >> B[i][j];
    }

    map<vector<int>, int> dist;

    auto create_field = [&](const vector<int> &v) -> vector<vector<int>> {
        vector res(H, vector<int>(W));
        for(int i=0; i<H; ++i){
            for(int j=0; j<W; ++j){
                res[i][j] = A[v[i]][v[H+j]];
            }
        }
        return res;
    };

    auto same_field = [&](const vector<vector<int>> &S, const vector<vector<int>> &T) -> bool {
        bool res = true;
        for(int i=0; i<H; ++i){
            for(int j=0; j<W; ++j){
                res &= (S[i][j] == T[i][j]);
            }
        }
        return res;
    };

    auto BFS = [&](vector<int> &s) -> int {
        queue<vector<int>> que;
        dist[s] = 0;
        que.push(s);

        while(!que.empty()){
            // キューの先頭をとりだす
            auto vec = que.front();
            int d = dist[vec];
            que.pop();

            // 取り出した盤面が B に一致していたらおわり
            vector<vector<int>> F = create_field(vec);
            if(same_field(F, B)) return d;

            // 一手先の盤面をすべて探索
            for(int i=0; i<H-1; ++i){
                swap(vec[i], vec[i+1]);
                // 過去にこの盤面を調べていたら，キューに追加しない
                if(dist.count(vec)){
                    swap(vec[i], vec[i+1]);
                    continue;
                }
                dist[vec] = d + 1;
                que.push(vec);
                swap(vec[i], vec[i+1]);
            }
            for(int i=0; i<W-1; ++i){
                swap(vec[H+i], vec[H+i+1]);
                if(dist.count(vec)){
                    swap(vec[H+i], vec[H+i+1]);
                    continue;
                }
                dist[vec] = d + 1;
                que.push(vec);
                swap(vec[H+i], vec[H+i+1]);
            }
        }
        // B に一致するものが見つからなかったらおわり
        return -1;
    };

    // 盤面を一次元配列つで保持 v[0..H-1]: 行の情報, v[H..H+W-1]: 列の情報
    vector<int> v(H+W);
    for(int i=0; i<H; ++i) v[i] = i;
    for(int i=0; i<W; ++i) v[H+i] = i;

    cout << BFS(v) << endl;
}