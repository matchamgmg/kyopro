#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int N;
    cin >> N;
    vector<int> A;
    int s;
    int a;
    rep(i, N)
    {
        cin >> a;
        A.push_back(a);
        s = A.size();
        while (true)
        {
            if (s <= 1)
                break;
            else if (A.at(s - 1) != A.at(s - 2))
                break;
            else
            {
                A.pop_back();
                s--;
                A.at(s - 1)++;
            }
        }
    }
    cout << s << endl;
}