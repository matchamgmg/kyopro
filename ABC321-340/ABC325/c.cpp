#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int h, w;
vector<string> s;
vector<vector<bool>> checked;

int check(int x, int y)
{
    if (checked.at(y).at(x) == true)
    {
        return 0;
    }
    if (s.at(y).at(x) == '.')
    {
        checked.at(y).at(x) = true;
        return 0;
    }
    checked.at(y).at(x) = true;
    
    if (x + 1 < w)
    {
        check(x + 1, y);
    }
    if (x + 1 < w && y - 1 >= 0)
    {
        check(x + 1, y - 1);
    }
    if (y - 1 >= 0)
    {
        check(x, y - 1);
    }
    if (x - 1 >= 0 && y - 1 >= 0)
    {
        check(x - 1, y - 1);
    }
    if (x - 1 >= 0)
    {
        check(x - 1, y);
    }
    if (x - 1 >= 0 && y + 1 < h)
    {
        check(x - 1, y + 1);
    }
    if (y + 1 < h)
    {
        check(x, y + 1);
    }
    if (x + 1 < w && y + 1 < h)
    {
        check(x + 1, y + 1);
    }

    return 1;
}

int main()
{
    cin >> h >> w;
    string ss;
    rep(i, h)
    {
        cin >> ss;
        s.push_back(ss);
    }
    vector<bool> c(w,false);
    rep(i,h){
        checked.push_back(c);
    }
    int ans = 0;
    rep(i, h)
    {
        rep(j, w)
        {
            ans += check(j, i);
        }
    }
    cout << ans << endl;
}