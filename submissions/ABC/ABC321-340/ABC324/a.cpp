#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n;
    cin>>n;
    int a;
    cin >> a;
    int b;
    rep(i,n-1){
        cin>>b;
        if(b!=a){
            cout << "No" << endl;
            return 0;
        }
    }
    cout << "Yes" << endl;
}