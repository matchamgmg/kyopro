#include <bits/stdc++.h>
using namespace std;

int main()
{
    int sum_a = 0;
    int sum_b = 0;
    vector<int> A(9), B(8);
    for (int i = 0; i < 9; i++)
    {
        cin >> A.at(i);
        sum_a += A.at(i);
    }
    for (int i = 0; i < 8; i++)
    {
        cin >> B.at(i);
        sum_b += B.at(i);
    }
    cout << sum_a - sum_b + 1 << endl;
}