#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()

int main()
{
    int n,m;
    cin >> n >> m;
    vector<int> A(n);
    rep(i,n){
        cin >> A.at(i);
    }
    sort(ALL(A));
    int ans = 0,cnt;
    auto itr = begin(A);
    int j = 0;
    rep(i,n){
       while(j<n&&A.at(j)<A.at(i)+m){
        j++;
       }
       cnt = j-i;
       ans = max(ans,cnt);
    }
    cout << ans << endl;
}