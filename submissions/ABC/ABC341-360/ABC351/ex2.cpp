#include <bits/stdc++.h>
using namespace std;

int main()
{
    int N;
    cin >> N;
    char c;
    vector<vector<char>> A(N, vector<char>(N));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> A.at(i).at(j);
        }
    }
    int a, b;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> c;
            if (c != A.at(i).at(j))
            {
                a = i + 1;
                b = j + 1;
            }
        }
    }
    cout << a << " " << b << endl;
}