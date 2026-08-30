#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    string s;
    cin >> s;
    int cntA = 0;
    int cnta = 0;
    string ans;
    rep(i,s.size()){
        if(islower(s.at(i))){
            cnta++;
        }else{
            cntA++;
        }
    }
    if(cntA>cnta){
        rep(i,s.size()){
            ans.push_back(toupper(s.at(i)));
        }
    }else{
        rep(i,s.size()){
            ans.push_back(tolower(s.at(i)));
        }
    }
    cout << ans << endl;
}