#include <bits/stdc++.h>
using namespace std;
int main()
{
    int N;
    cin >> N;
    int sum = 0, a;
    for (int i = 0; i < N - 1; i++)
    {
        cin >> a;
        sum += a;
    }
    cout << 0 - sum << endl;
}