#include <bits/stdc++.h>
using namespace std;
int main()
{
    string s, t;
    cin >> s >> t;
    bool code = false;
    if (t.at(2) == 'X')
    {
        for (int i = 0, j = 0; i < s.size(); i++)
        {
            if (s.at(i) == tolower(t.at(j)))
            {
                j++;
                if (j > 1)
                {
                    code = true;
                    break;
                }
            }
        }
    }
    else
    {
        for (int i = 0, j = 0; i < s.size(); i++)
        {
            if (s.at(i) == tolower(t.at(j)))
            {
                j++;
                if (j > 2)
                {
                    code = true;
                    break;
                }
            }
        }
    }
    cout << (code ? "Yes" : "No") << endl;
}