#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int n;
    cin >> n;
    deque<pair<int, int>> L;
    pair<int,int> ans;
    rep(i, n)
    {
        L.push_back(make_pair(i + 1, 0));
    }
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
            L.pop_back();
            L.push_front(make_pair(L.front().first+dx.at(forward),L.front().second+dy.at(forward)));
        }
        else if (q0 == 2)
        {
            cin >> q2;
            ans = L.at(q2-1);
            cout << ans.first << " " << ans.second << endl;
        }
    }
}