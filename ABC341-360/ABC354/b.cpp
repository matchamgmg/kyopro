#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int N;
    cin >> N;
    vector<pair<string,int>> A(N,pair<string,int>());
    int rating =0;
    rep(i,N){
        cin >>A.at(i).first>>A.at(i).second;
        rating+=A.at(i).second;
    }
    sort(A.begin(),A.end());
    rep(i,N){
        if(i==rating%N){
            cout<<A.at(i).first<<endl;
            return 0;
        }
    }
}