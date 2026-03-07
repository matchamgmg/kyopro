#include <bits/stdc++.h>
#include <atcoder/all>
#define rep(i, s, n) for (int i = s; i < (n); i++)
#define ALL(a) a.begin(),a.end()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
using namespace std;
using namespace atcoder;
using ll = long long;
using pii = pair<int,int>;
using vi = vector<int>;
using vll = vector<ll>;
using vpii = vector<pii>;
using vvi = vector<vi>;
using vvll = vector<vll>;
using Graph = vvi;
using wGraph = vector<vpii>;

int main()
{
    int N;
    cin >> N;
    vector<vector<char>> A0(N,vector<char>(N));
    vector<vector<char>> A1(N,vector<char>(N));
    vector<vector<char>> A2(N,vector<char>(N));
    vector<vector<char>> A3(N,vector<char>(N));
    vector<vector<char>> Ans(N,vector<char>(N,'a'));
    rep(i,0,N){
        rep(j,0,N){
            cin >> A0[i][j];
        }
    }
    //cout << "aa" << endl;
    rep(i,0,N){
        rep(j,0,N){
            A1[j][N-1-i] = A0[i][j];
        }
    }
    rep(i,0,N){
        rep(j,0,N){
            A2[j][N-1-i] = A1[i][j];
        }
    }
    rep(i,0,N){
        rep(j,0,N){
            A3[j][N-1-i] = A2[i][j];
        }
    }
    rep(i,0,N/2){
        rep(j,i,N/2){
            if(i%4==0){
                Ans[i][j] = A1[i][j];
                Ans[i][N-1-j] = A1[i][N-1-j];
                Ans[N-1-i][j] = A1[N-1-i][j];
                Ans[N-1-i][N-1-j] = A1[N-1-i][N-1-j];
            }
            
            if(i%4==1){
                Ans[i][j] = A2[i][j];
                Ans[i][N-1-j] = A2[i][N-1-j];
                Ans[N-1-i][j] = A2[N-1-i][j];
                Ans[N-1-i][N-1-j] = A2[N-1-i][N-1-j];
            }
            if(i%4==2){
                Ans[i][j] = A3[i][j];
                Ans[i][N-1-j] = A3[i][N-1-j];
                Ans[N-1-i][j] = A3[N-1-i][j];
                Ans[N-1-i][N-1-j] = A3[N-1-i][N-1-j];
            }
            if(i%4==3){
                Ans[i][j] = A0[i][j];
                Ans[i][N-1-j] = A0[i][N-1-j];
                Ans[N-1-i][j] = A0[N-1-i][j];
                Ans[N-1-i][N-1-j] = A0[N-1-i][N-1-j];
            }
            
        }
    }
    rep(j,0,N/2){
        rep(i,j,N/2){
            if(j%4==0){
                Ans[i][j] = A1[i][j];
                Ans[i][N-1-j] = A1[i][N-1-j];
                Ans[N-1-i][j] = A1[N-1-i][j];
                Ans[N-1-i][N-1-j] = A1[N-1-i][N-1-j];
            }
            
            if(j%4==1){
                Ans[i][j] = A2[i][j];
                Ans[i][N-1-j] = A2[i][N-1-j];
                Ans[N-1-i][j] = A2[N-1-i][j];
                Ans[N-1-i][N-1-j] = A2[N-1-i][N-1-j];
            }
            if(j%4==2){
                Ans[i][j] = A3[i][j];
                Ans[i][N-1-j] = A3[i][N-1-j];
                Ans[N-1-i][j] = A3[N-1-i][j];
                Ans[N-1-i][N-1-j] = A3[N-1-i][N-1-j];
            }
            if(j%4==3){
                Ans[i][j] = A0[i][j];
                Ans[i][N-1-j] = A0[i][N-1-j];
                Ans[N-1-i][j] = A0[N-1-i][j];
                Ans[N-1-i][N-1-j] = A0[N-1-i][N-1-j];
            }
            
        }
    }
    /*
    rep(i,0,N){
        rep(j,0,N){
            cout << A0[i][j];
        }
        cout << endl;
    }
    cout << endl;
    rep(i,0,N){
        rep(j,0,N){
            cout << A1[i][j];
        }
        cout << endl;
    }
    cout << endl;
    rep(i,0,N){
        rep(j,0,N){
            cout << A2[i][j];
        }
        cout << endl;
    }
    cout << endl;
    rep(i,0,N){
        rep(j,0,N){
            cout << A3[i][j];
        }
        cout << endl;
    }
    cout << endl;
    */
    rep(i,0,N){
        rep(j,0,N){
            cout << Ans[i][j];
        }
        cout << endl;
    }
}