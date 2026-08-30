#include <bits/stdc++.h>
using namespace std;
int main()
{
    string S, T;
    cin >> S >> T;
    vector<int> a;
    for (int i = 0, j = 0; i < S.length(); i++)
    {
        while (true)
        {
            if (T.at(j) == S.at(i))
            {
                a.push_back(j + 1);
                j++;
                break;
            }
            j++;
        }
    }
    for (int i = 0; i < a.size() - 1; i++)
    {
        cout << a.at(i) << " ";
    }
    cout << a.at(a.size()-1) << endl;
}