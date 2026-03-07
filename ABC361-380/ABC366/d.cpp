#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<vector<vector<int>>> A(N,vector<vector<int>>(N,vector<int>(N)));
    rep(i,N){
        rep(j,N){
            rep(k,N){
                cin >> A[i][j][k];
            }
        }
    }
    //累積和
    vector<vector<vector<int>>> S(N+1,vector<vector<int>>(N+1,vector<int>(N+1,0)));
    rep(i,N){
        rep(j,N){
            rep(k,N){
                S[i+1][j+1][k+1] = S[i+1][j+1][k] + S[i][j+1][k+1] + S[i+1][j][k+1]
                - S[i+1][j][k] - S[i][j+1][k] - S[i][j][k+1] + S[i][j][k]
                + A[i][j][k];
            }
        }
    }

    int Q;
    cin >> Q;
    vector<int> Lx(Q),Rx(Q),Ly(Q),Ry(Q),Lz(Q),Rz(Q);
    rep(i,Q){
        cin >> Lx[i] >> Rx[i] >> Ly[i] >> Ry[i] >> Lz[i] >> Rz[i];
    }
    rep(i,Q){
        int ans = S[Rx[i]][Ry[i]][Rz[i]] - S[Rx[i]][Ry[i]][Lz[i]-1]
        - S[Lx[i]-1][Ry[i]][Rz[i]] - S[Rx[i]][Ly[i]-1][Rz[i]]
        + S[Rx[i]][Ly[i]-1][Lz[i]-1] + S[Lx[i]-1][Ry[i]][Lz[i]-1] + S[Lx[i]-1][Ly[i]-1][Rz[i]]
        - S[Lx[i]-1][Ly[i]-1][Lz[i]-1];

        cout << ans << endl;
    }
}