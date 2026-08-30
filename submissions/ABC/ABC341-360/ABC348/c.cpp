#include <bits/stdc++.h>
using namespace std;
int main()
{
    int N;
    cin >> N;
    map<int, int> CA;
    int c, a;
    for (int i = 0; i < N; i++)
    {
        cin >> a >> c;
        if (!CA.count(c))
        {
            CA[c] = a;
        }
        else if (CA.at(c) > a)
        {
            CA.at(c) = a;
        }
    }
    int max_a = 0;
    for (auto aa : CA)
    {
        if (max_a < aa.second)
        {
            max_a = aa.second;
        }
    }
    cout << max_a << endl;
}