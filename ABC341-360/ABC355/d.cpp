#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n;
    cin >> n;
    vector<pair<int,int>> lr;
    vector<int> c;
    int a,b;
    rep(i,n){
        cin >> a >> b;
        lr.push_back(make_pair(a,b));
        c.push_back(a);
    }
    sort(lr.begin(),lr.end());
    sort(c.begin(),c.end());
    int ans = 0;
    rep(i,n){
        auto itr = upper_bound(c.begin(),c.end(),lr.at(i).second);
        int dis = distance(c.begin(),itr);
        ans += dis - i-1;
    }
    cout << ans << endl;
}