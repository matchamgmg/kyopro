#include <bits/stdc++.h>
using namespace std;

int64_t c2(int64_t x)
{
    return (int64_t)x * (x - 1) / 2;
}
int main()
{
    string S;
    cin >> S;
    int s = S.size();
    vector<int> A(26, 0);
    for (char ss : S)
    {
        A.at(ss - 'a')++;
    }
    int64_t same, diff;
    same = 0;
    for (int a : A)
    {
        same += c2(a);
    }
    diff = c2(s) - same;
    int64_t ans = diff;
    if (same > 0)
        ans++;
    cout << ans << endl;
}