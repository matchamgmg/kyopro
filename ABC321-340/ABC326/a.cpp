#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int x, y;
    cin >> x >> y;
    if (y<=x+2&&y>=x-3)
    {
        cout << "Yes" << endl;
    }
    else
    {
        cout << "No" << endl;
    }
}