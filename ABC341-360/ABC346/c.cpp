#include <bits/stdc++.h>
using namespace std;
int main()
{
    int N, K;
    cin >> N >> K;
    set<int> A;
    int a;
    for (int i = 0; i < N; i++)
    {
        cin >> a;
        A.insert(a);
    }
    int64_t ans = (int64_t)(K + 1) * K / 2;
    for (int aa : A)
    {
        if (aa <= K)
            ans -= aa;
    }
    cout << ans << endl;
}