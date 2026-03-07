#include <bits/stdc++.h>
using namespace std;
int main()
{
    string s;
    cin >> s;
    const int alp = 26;
    vector<int> count(alp, 0);
    for (int i = 0; i < s.size(); i++)
    {
        count.at(s.at(i) - 'a')++;
    }
    int count2;
    bool good = true;
    for (int i = 1; i < s.size() + 1; i++)
    {
        count2 = 0;
        for (int j = 0; j < alp; j++)
        {
            if (count.at(j) == i)
            {
                count2++;
            }
        }
        if (count2 != 0 && count2 != 2)
        {
            good = false;
        }
    }
    cout << (good ? "Yes" : "No") << endl;
}