#include <bits/stdc++.h>
using namespace std;
int main()
{
    string s;
    cin >> s;
    bool a = true;
    for (int i = 0; i < s.size(); i++)
    {
        if (i == 0)
        {
            if (s.at(i) != '<')
                a = false;
        }
        else if (i == s.size() - 1)
        {
            if (s.at(i) != '>')
                a = false;
        }
        else
        {
            if (s.at(i) != '=')
                a = false;
        }
    }
    cout << (a?"Yes":"No") << endl;
}