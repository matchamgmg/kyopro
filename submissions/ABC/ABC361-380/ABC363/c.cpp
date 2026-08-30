#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N, K;
    string S;
    cin >> N >> K >> S;
    sort(ALL(S));
    ll cnt = 0;
    do{
        //cout << S << endl;
        bool check2 = false;
        for(int i=0;i+K-1<N;i++){
            bool check = true;
            rep(j,K){
                if(S[i+j]!=S[i+K-j-1]){
                    check = false;
                }
            }
            if(check){
                check2 = true;
            }
        }
        //cout << (check2?"o":"x") << endl;
        if(!check2){
            cnt++;
        }
    }while(next_permutation(ALL(S)));
    cout << cnt << endl;
}