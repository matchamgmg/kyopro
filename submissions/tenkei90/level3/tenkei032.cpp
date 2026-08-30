#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

/*
bool check(vector<int> jun,vector<int> X,vector<int> Y){
    rep(i,X.size()){
        rep(j,jun.size()-1){
            if((jun[j]==X[i]&&jun[j+1]==Y[i])||(jun[j]==Y[i]&&jun[j+1]==X[i])){
                return false;
            }
        }
    }
    return true;
}
*/

int main()
{
    int N;
    cin >> N;
    vector<vector<int>> A(N,vector<int>(N,0));
    rep(i,N) rep(j,N) cin >> A[i][j];
    int M;
    cin >> M;
    /*
    vector<int> X(M),Y(M);
    rep(i,M) {
        cin >> X[i] >> Y[i];
        X[i]--;
        Y[i]--;
    }
    */
    vector<vector<bool>> kenaku(N,vector<bool>(N,false));
    int x,y;
    rep(i,M){
        cin >> x >> y;
        kenaku[x-1][y-1] = true;
        kenaku[y-1][x-1] = true;
    }
    vector<int> jun(N,0);
    rep(i,N) jun[i] = i;
    int ans = 1<<20;
    do{
        /*
        if(check(jun,X,Y)){
            int time = 0;
            rep(i,N){
                time += A[jun[i]][i];
            }
            ans = min(ans,time);
        }
        */
        int time = 0;
        bool flag = true;
        rep(i,N){
            if(i!=N-1&&kenaku[jun[i]][jun[i+1]]){
                flag = false;
                break;
            }
            else time += A[jun[i]][i];
        }
        if(flag) ans = min(ans,time);
    }while(next_permutation(ALL(jun)));
    if(ans!=1<<20) cout << ans << endl;
    else cout << -1 << endl;
}