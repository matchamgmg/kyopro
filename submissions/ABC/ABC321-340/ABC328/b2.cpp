#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n;
    cin >> n;
    vector<int> D(n);
    rep(i,n){
        cin >> D.at(i);
    }
    string t,tt,ttt;
    int cnt=0;
    rep(i,n){
        t = to_string(i+1);
        rep(j,D.at(i)){
            tt = to_string(j+1);
            ttt = t + tt;
            if(size(set<char>(ttt.begin(),ttt.end()))==1)cnt++;
        }
    }
    cout << cnt << endl;
}