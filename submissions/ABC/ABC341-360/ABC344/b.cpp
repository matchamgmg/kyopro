#include <bits/stdc++.h>
using namespace std;
int main()
{
    stack<int> A;
    int a;
    while (true)
    {
        cin >> a;
        A.push(a);
        if (a == 0)
            break;
    }
    while (!A.empty())
    {
        cout << A.top() << endl;
        A.pop();
    }
}