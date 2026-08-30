#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<string> S(N);
    int max_len = 0;
    rep(i,N){
        cin >> S[i];
        max_len = max(max_len,(int)S[i].size());
    }
    vector<string> T(max_len);
    for(int i=N-1;i>=0;i--){
        rep(j,max_len){
            if(j<(int)S[i].size()){
                if((int)T[j].size()<N-i-1){
                    int s = N-i-1-(int)T[j].size();
                    for(int k=0;k<s;k++){
                        T[j].push_back('*');
                    }
                }
                T[j].push_back(S[i][j]);
            }
           // cout << T[j] << endl;
        }
    }

    rep(i,max_len){
        cout << T[i] << endl;
    }
}