#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int n;
    cin>>n;
    int i=0;
    int ans=0;
    while(n>1>>i){
        if((n>>i&1)==0){
            ans++;
            i++;
        }else{
            break;
        }
    }
    cout<<ans<<endl;
}