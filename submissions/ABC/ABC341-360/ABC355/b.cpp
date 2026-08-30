#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n,m;
    cin >>n>>m;
    vector<pair<int,int>> C;
    int s;
    rep(i,n){
        cin >> s;
        C.push_back(make_pair(s,0));
    }
    rep(i,m){
        cin>>s;
        C.push_back(make_pair(s,1));
    }
    sort(C.begin(),C.end());
    rep(i,C.size()){
        if(i!=0){
            if(C.at(i-1).second==0&&C.at(i).second==0){
                cout << "Yes" << endl;
                return 0;
            }
        }
    }
    cout << "No" << endl;
}