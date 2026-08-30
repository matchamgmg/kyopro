#include <bits/stdc++.h>
using namespace std;
int main()
{
    int N;
    cin >> N;
    string s;
    for (int i = 0; i < N; i++)
    {
        i % 3 == 2 ? s.push_back('x') : s.push_back('o');
    }
    cout << s << endl;
}