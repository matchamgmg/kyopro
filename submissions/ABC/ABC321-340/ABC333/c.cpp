#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)
using ll = long long;


int main()
{
    int n;
    cin >> n;
    ll a,b,c;
    int i,j,k;
    int count = 0;
    a = 0;
    for(i=0;count<n;i++){
        a = a*10+1;
        b = 0;
        for(j=0;b<a&&count<n;j++){
            b = b*10+1;
            c = 0;
            for(k=0;c<b&&count<n;k++){
                c = c*10+1;
                count++;
            }
        }
    }
    cout << a+b+c << endl;
}