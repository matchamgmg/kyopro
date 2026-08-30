#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n,m;
    string s,t;
    cin>>n>>m>>s>>t;
    bool pre=true,suf=true;
    rep(i,n){
        if(s.at(i)!=t.at(i)){
            pre = false;
            break;
        }
    }
    rep(i,n){
        if(s.at(n-1-i)!=t.at(m-1-i)){
            suf = false;
            break;
        }
    }
    cout << (pre&&suf?0:pre?1:suf?2:3) << endl;
}