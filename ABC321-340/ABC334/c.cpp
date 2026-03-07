#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n,k;
    cin >>n>>k;
    vector<int> A(k);
    vector<int> C(n,2);
    int a;
    rep(i,k){
        cin >> a;
        a--;
        C.at(a)--;
    }
    vector<int> x;
    rep(i,n)rep(j,C.at(i)){
        x.push_back(i);
    }
    n = x.size();//靴下の枚数

    if(k%2==0){
        int ans = 0;
        rep(i,n/2){
            ans += x.at(2*i+1)-x.at(2*i);
        }
        cout << ans << endl;
    }else{
        int now = 0;
        rep(i,n/2){
            now += x.at(2*i+2) - x.at(2*i+1);
        }
        int ans = now;
        for(int i=2;i<n;i=i+2){
            now += x.at(i-1)-x.at(i-2);
            now -= x.at(i)-x.at(i-1);
            ans = min(ans,now);
        }
        cout << ans << endl;
    }
}