#include <bits/stdc++.h>
using namespace std;
int main()
{
    int N, Q;
    cin >> N >> Q;
    vector<int> T(N, 1);
    int t, sum = 0;
    for (int i = 0; i < Q; i++)
    {
        cin >> t;
        T.at(t - 1) = (T.at(t - 1) + 1) % 2;
    }
    for (int i = 0; i < N; i++)
    {
        sum += T.at(i);
    }
    cout << sum << endl;
}