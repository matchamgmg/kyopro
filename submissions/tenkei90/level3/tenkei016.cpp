#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    ll N,A,B,C;
    cin >> N >> A >> B >> C;
    int min_coins = 1<<20;
    for(int i=0;i<10000;i++){
        if(A*i>N) continue;
        for(int j=0;i+j<10000;j++){
            if(B*j>N) continue;
            int c = N - (A*i+B*j);
            if(c<0) continue;
            int k = c/C;
            int rem = c%C;
            if(rem==0){
                min_coins = min(min_coins,i+j+k);
            }
        }
    }
    cout << min_coins << endl;
}