#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int H, W, N;
    cin >> H >> W >> N;
    vector<vector<char>> A(H, vector<char>(W, '.'));
    int x = 0, y = 0;
    int forward = 0; // 0,1,2,3
    rep(i, N)
    {
        if (A.at(y).at(x) == '.')
        {
            A.at(y).at(x) = '#';
            forward = (forward + 1) % 4;
        }
        else
        {
            A.at(y).at(x) = '.';
            forward = (forward + 4 - 1) % 4;
        }
        switch (forward)
        {
        case 0:
            y = (y + H - 1) % H;
            break;
        case 1:
            x = (x + 1) % W;
            break;
        case 2:
            y = (y + 1) % H;
            break;
        case 3:
            x = (x + W - 1) % W;
            break;
        }
    }
    rep(i, H)
    {
        rep(j, W)
        {
            cout << A.at(i).at(j);
        }
        cout << endl;
    }
}