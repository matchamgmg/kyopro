#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)


int main()
{
    int n;
    cin >> n;
    int i=0,j=0,k=0;
    int a,b,c;
    int count = 0;
    for(i=0;count<n;i++){
        a=i;
        for(j=0;j<=i&&count<n;j++){
            b=j;
            for(k=0;k<=j&&count<n;k++){
                c=k;
                count++;
            }
        }
    }
    cout << string(a-b,'1') + string(b-c,'2') + string(c+1,'3') << endl;
}