#include <bits/stdc++.h>
using namespace std;
int main()
{
    int N;
    cin >> N;
    vector<int> A(N);
    for (int i = 0; i < N; i++)
    {
        cin >> A.at(i);
    }
    int count = 0;
    vector<int> a, b;
    for (int i = 0; i < N - 1; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            if (i + 1 == A.at(i))
            {
                break;
            }
            if (i + 1 == A.at(j))
            {
                swap(A.at(i), A.at(j));
                a.push_back(i + 1);
                b.push_back(j + 1);
                count++;
                break;
            }
        }
    }
    cout << count << endl;
    for (int i = 0; i < a.size(); i++)
    {
        cout << a.at(i) << " " << b.at(i) << endl;
    }
}