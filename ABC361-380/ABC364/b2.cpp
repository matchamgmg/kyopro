#include<bits/stdc++.h>
using namespace std;

// よく使う表現はマクロ化すると便利
// 他人のコードのいい感じのマクロは積極的にパクろう
#define rep(i,s,n) for(int i=s ; i<(int)n ; i++)
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

int main(){
    /* 入力を受け取る */
    int H,W;
    cin >> H >> W;
    int Si,Sj;
    cin >> Si >> Sj;
    Si--;
    Sj--;
    vector<string> C(H);
    rep(i,0,H){
        cin >> C[i];
    }
    string X;
    cin >> X;

    /* 4方向の移動をfor文で書くための準備 */
    // グリッドの上からi番目，左からj番目のマスをマス(i,j)と表す
    // 現在のマスが(x,y)で，Xのi文字目が muki[j] に等しいとき
    // 移動先のマスは，(x + dx[j], y + dy[y])

    // となるように配列muki,dx,dyを決める
    vector<char> muki = {'L','R','U','D'};
    vector<int> dx = {0,0,-1,1};
    vector<int> dy = {-1,1,0,0};

    /* シミュレーション */
    int x = Si;
    int y = Sj;

    rep(i,0,X.size()){  // Xを一文字ずつ見ていく
        rep(j,0,4){     // Xのi文字目がmukiのj文字目に一致するかを調べる
            if(X[i]==muki[j]){  //一致していたら移動する処理を行う
                int next_x = x + dx[j];
                int next_y = y + dy[j];
                // 移動先がグリッドの範囲外なら何もしない
                if(!grid_check(next_x,next_y,H,W)) continue;    // 上で定義したマクロ．グリッドの範囲内かを判定する条件式
                // 移動先が'#'なら何もしない
                if(C[next_x][next_y]=='#') continue;
                // 現在のマスを更新する
                x = next_x;
                y = next_y;
            }
        }
    }

    /* 出力 */
    cout << (x+1) << " " << (y+1) << endl;
}