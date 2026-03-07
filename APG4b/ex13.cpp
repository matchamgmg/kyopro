#include <bits/stdc++.h>
using namespace std;

int main()
{
    int N;
    cin >> N;
    vector<int> A(N);
    int mean = 0;

    for (int i = 0; i < N; i++)
    {
        cin >> A.at(i);
        mean += A.at(i);
    }

    mean /= N;

    for (int i = 0; i < N; i++)
    {
        cout << abs(A.at(i) - mean) << endl;
    }
}
