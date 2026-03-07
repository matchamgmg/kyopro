#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n,S,M,L;
    cin >>n>>S>>M>>L;
    int ans;
    int count =0;
    rep(i,n+1){
        rep(j,n+1){
            rep(k,n+1){
                if(6*i+8*j+12*k>=n){
                    if(!count){
                        count++;
                        ans = S*i+M*j+L*k;
                    }else{
                        ans = min(ans,S*i+M*j+L*k);
                    }
                }
            }
        }
    }
    cout << ans << endl;
}