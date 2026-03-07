#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int H,W;
    cin >> H >> W;
    int si,sj;
    cin >> si >> sj;
    si -= 1;
    sj -= 1;
    vector<vector<char>> C(H,vector<char>(W));
    rep(i,H){
        rep(j,W){
            cin >> C[i][j];
        }
    }
    string X;
    cin >> X;
    vector<int> dx = {1,0,-1,0};
    vector<int> dy = {0,1,0,-1};
    int idx = -1;
    rep(i,X.size()){
        if(X[i]=='U'){
            idx = 2;
        }else if(X[i]=='D'){
            idx = 0;
        }else if(X[i]=='L'){
            idx = 3;
        }else if(X[i]=='R'){
            idx = 1;
        }
        if(si+dx[idx]>=0&&si+dx[idx]<H&&sj+dy[idx]>=0&&sj+dy[idx]<W&&C[si+dx[idx]][sj+dy[idx]]=='.'){
            si += dx[idx];
            sj += dy[idx];
        }
    }
    cout << (si+1) << " " << (sj+1) << endl;
}