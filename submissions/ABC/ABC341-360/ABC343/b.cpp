#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int N;
    cin >> N;
    vector<vector<int>> A(N,vector<int>(N));
    rep(i,N){
        rep(j,N){
            cin >> A.at(i).at(j);
        }
    }
    rep(i,N){
        rep(j,N){
            if(A.at(i).at(j)==1) cout << j+1 << " "; 
        }
        cout << endl;
    }
}