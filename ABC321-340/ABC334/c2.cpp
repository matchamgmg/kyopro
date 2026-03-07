#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n,k;
    cin >>n>>k;
    vector<int> A(k); //1枚の靴下の色昇順
    rep(i,k){
        cin >> A.at(i);
    }

    if(k%2==0){
        int ans = 0;
        rep(i,k/2){
            ans += A.at(2*i+1)-A.at(2*i);
        }
        cout << ans << endl;
    }else{
        int now = 0;
        rep(i,k/2){
            now += A.at(2*i+2) - A.at(2*i+1);
        }
        int ans = now;
        for(int i=2;i<k;i=i+2){
            now += A.at(i-1)-A.at(i-2);
            now -= A.at(i)-A.at(i-1);
            ans = min(ans,now);
        }
        cout << ans << endl;
    }
}