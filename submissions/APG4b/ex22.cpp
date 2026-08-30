#include <bits/stdc++.h>
using namespace std;

int main()
{
    int N;
    cin >> N;
    using pii = pair<int, int>;
    vector<pii> AB(N);
    for (pii &ab : AB)
    {
        cin >> ab.second >> ab.first;
    }
    sort(AB.begin(), AB.end());
    for (pii &ab : AB)
    {
        cout << ab.second << " " << ab.first << endl;
    }
}