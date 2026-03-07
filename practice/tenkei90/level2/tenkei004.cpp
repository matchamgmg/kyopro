#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int H,W;
    cin >> H >> W;
    vector<vector<int>> A(H,vector<int>(W));
    vector<int> tate_sum(W,0),yoko_sum(H,0);
    vector<vector<int>> ans(H,vector<int>(W));
    int input;
    rep(i,H){
        rep(j,W){
            cin >> input;
            A.at(i).at(j) = input;
            tate_sum.at(j) += input;
            yoko_sum.at(i) += input;
        }
    }
    rep(i,H){
        rep(j,W){
            cout << tate_sum.at(j) + yoko_sum.at(i) - A.at(i).at(j) << " ";
        }
        cout << endl;
    }
}