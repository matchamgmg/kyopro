#include <bits/stdc++.h>
using namespace std;

int main()
{
    int N, A;
    cin >> N >> A;

    int B;
    string op;
    for (int i = 0; i < N; i++)
    {
        cin >> op >> B;
        if (op == "+")
        {
            A += B;
            cout << i + 1 << ":";
            cout << A << endl;
        }
        else if (op == "-")
        {
            A -= B;
            cout << i + 1 << ":";
            cout << A << endl;
        }
        else if (op == "*")
        {
            A *= B;
            cout << i + 1 << ":";
            cout << A << endl;
        }
        else if (op == "/" && B != 0)
        {
            A /= B;
            cout << i + 1 << ":";
            cout << A << endl;
        }
        else
        {
            cout << "error" << endl;
            break;
        }
    }
}
