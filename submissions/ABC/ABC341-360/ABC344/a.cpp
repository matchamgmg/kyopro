#include <bits/stdc++.h>
using namespace std;
int main()
{
    string s;
    cin >> s;
    string ss = "";
    bool a = true;
    for (int i = 0; i < s.size(); i++)
    {
        if (s.at(i) == '|')
        {
            if (a)
            {
                ss += s.substr(0, i);
                a = false;
            }
            else
            {
                ss += s.substr(i + 1, s.size() - i - 1);
            }
        }
    }
    cout << ss << endl;
}