#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n,m;
    cin>>n>>m;
    vector<int> A(m);
    rep(i,m){
        cin>>A.at(i);
    }
    vector<int> ans;
    int j = m-1;
    rep(i,n){
        if(i==0){
            ans.push_back(0);
            j--;
        }else{
            if(j>=0&&A.at(j)==n-i){
                ans.push_back(0);
                j--;
            }else{
                ans.push_back(ans.back()+1);
            }
        }
    }
    rep(i,n){
        cout << ans.at(n-1-i) << " ";
    }
}