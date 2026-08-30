#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n;
    cin>>n;
    vector<int> A(n);
    int maxnum=0,max2=0;
    rep(i,n){
        cin>>A.at(i);
        maxnum=max(maxnum,A.at(i));
    }
    rep(i,n){
        if(A.at(i)<maxnum){
            max2=max(max2,A.at(i));
        }
    }
    cout<<max2<<endl;
}