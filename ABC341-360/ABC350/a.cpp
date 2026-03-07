#include <bits/stdc++.h>
using namespace std;
int main()
{
    string s;
    cin >> s;
    if (("ABC001" <= s && s <= "ABC315") || ("ABC317" <= s && s <= "ABC349"))
    {
        cout << "Yes" << endl;
    }
    else
    {
        cout << "No" << endl;
    }
}