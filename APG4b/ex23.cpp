#include <bits/stdc++.h>
using namespace std;

int main()
{
    int N, a;
    cin >> N;

    vector<int> A(N);
    for (auto &i : A)
    {
        cin >> i;
    }
    map<int, int> cnt;
    for (auto x : A)
    {
        if (cnt.count(x))
        {
            cnt.at(x)++;
        }
        else
        {
            cnt[x] = 1;
        }
    }
    int max_cnt = 0;
    int ans = -1;
    for (auto x : A)
    {
        if (max_cnt < cnt.at(x))
        {
            ans = x;
            max_cnt = cnt.at(x);
        }
    }
    cout << ans << " " << max_cnt << endl;
}