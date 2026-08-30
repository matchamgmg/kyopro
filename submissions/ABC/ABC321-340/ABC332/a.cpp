#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int n,s,k;
    cin>>n>>s>>k;
    int p,q;
    int sum=0;
    rep(i,n){
        cin>>p>>q;
        sum+=p*q;
    }
    cout << (sum>=s?sum:sum+k) << endl;
}