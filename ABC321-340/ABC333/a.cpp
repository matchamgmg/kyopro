#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int n;
    cin >> n;
    string s ="";
    rep(i,n){
        s+=to_string(n);
    }
    cout << s << endl;
}