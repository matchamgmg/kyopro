#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int R;
    cin >> R;
    int a = R/100;
    cout << (a*100 + 100 - R) << endl;
}