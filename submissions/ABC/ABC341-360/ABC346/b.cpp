#include <bits/stdc++.h>
using namespace std;
int main()
{
    int W, B;
    cin >> W >> B;
    string s = "wbwbwwbwbwbw";
    while (s.size() - 12 < W + B)
    {
        s += s;
    }
    string ss;
    int w, b;
    for (int i = 0; i < 12; i++)
    {
        for (int j = 1; j < s.size() - i; j++)
        {
            ss = s.substr(i, j);
            w = 0;
            b = 0;
            for (int k = 0; k < ss.size(); k++)
            {
                if (ss.at(k) == 'w')
                    w++;
                if (ss.at(k) == 'b')
                    b++;
            }
            if (w == W && b == B)
            {
                cout << "Yes" << endl;
                return 0;
            }
        }
    }
    cout << "No" << endl;
}