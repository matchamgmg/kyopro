#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int n;
    cin>>n;
    int zeros = 0;
    while(n>0){
        if(n%2==0){
            zeros++;
            n/=2;
        }else{
            break;
        }
    }
    cout << zeros << endl;
}