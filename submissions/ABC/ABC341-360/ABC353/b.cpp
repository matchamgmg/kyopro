#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int N, K;
    cin >> N >> K;
    queue<int> A;
    int a;
    rep(i, N)
    {
        cin >> a;
        A.push(a);
    }
    int count = 0;
    int k = K;
    while (!A.empty())
    {
        if (A.front() <= k)
        {
            k -= A.front();
            A.pop();
        }
        else
        {
            count++;
            k = K;
        }
    }
    count++;
    cout << count << endl;
}