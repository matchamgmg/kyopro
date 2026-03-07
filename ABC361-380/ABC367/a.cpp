#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int a,b,c;
    cin >> a >> b >> c;
    if((b<a&&a<c) || (b<a&&c<b) || (a<c&&c<b)){
        cout << "No" << endl;
    }else{
        cout << "Yes" << endl;
    }
}