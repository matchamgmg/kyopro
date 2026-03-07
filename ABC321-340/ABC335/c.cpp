#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int n;
    cin >> n;
    vector<pair<int, int>> L(n, pair<int, int>());
    pair<int,int> ans;
    rep(i, n)
    {
        L.at(i) = make_pair(i + 1, 0);
    }
    int count = 0;
    int q;
    cin >> q;
    int q0, q2;
    char q1;
    vector<int> dx = {1, 0, -1, 0};
    vector<int> dy = {0, 1, 0, -1};
    int forward;
    rep(i, q)
    {
        cin >> q0;
        if (q0 == 1)
        {
            cin >> q1;
            if (q1 == 'R')
            {
                forward = 0;
            }
            else if (q1 == 'U')
            {
                forward = 1;
            }
            else if (q1 == 'L')
            {
                forward = 2;
            }
            else if (q1 == 'D')
            {
                forward = 3;
            }
            L.at(n-1-count).first = L.at((n-count)%n).first + dx.at(forward);
            L.at(n-1-count).second = L.at((n-count)%n).second + dy.at(forward);
            count = (count + 1)%n;
        }
        else if (q0 == 2)
        {
            cin >> q2;
            ans = L.at((q2-1-count+n)%n);
            cout << ans.first << " " << ans.second << endl;
        }
    }
}