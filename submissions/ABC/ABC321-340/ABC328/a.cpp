#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n,x;
    cin>>n>>x;
    int sum=0,s;
    rep(i,n){
        cin>>s;
        if(s<=x){
            sum+=s;
        }
    }
    cout<<sum<<endl;
}