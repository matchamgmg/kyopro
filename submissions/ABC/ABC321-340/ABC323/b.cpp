#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n;
    cin >> n;
    vector<string> s(n);
    rep(i,n){
        cin >> s.at(i);
    }
    vector<pair<int,int>> ans(n,pair<int,int>());
    rep(i,n){
        int cnt = 0;
        rep(j,n){
            if(s.at(i).at(j)=='o'){
                cnt++;
            }
        }
        ans.at(i) = make_pair(cnt,n-(i+1));
    }
    sort(ans.begin(),ans.end(),greater<pair<int,int>>{});
    rep(i,n){
        cout << n - ans.at(i).second << endl;
    }
}