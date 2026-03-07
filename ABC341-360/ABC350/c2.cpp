#include <bits/stdc++.h>
using namespace std;
int main()
{
    int N;
    cin >> N;
    vector<int> A(N), P(N);
    int k;
    for (int i = 0; i < N; i++)
    {
        cin >> k;
        A.at(i) = k - 1;
        P.at(k - 1) = i;
    }
    int count = 0;
    queue<int> a, b;
    int tmp1, tmp2;
    for (int i = 0; i < N; i++)
    {
        tmp1 = P.at(A.at(i)); //=i
        tmp2 = P.at(i);       //=j
        if (tmp1 != tmp2)
        {
            a.push(tmp1);
            b.push(tmp2);
            P.at(A.at(i)) = tmp2;
            P.at(i) = tmp1;
            swap(A.at(tmp1), A.at(tmp2));
            count++;
        }
    }
    cout << count << endl;
    while (!a.empty())
    {
        cout << a.front() + 1 << " " << b.front() + 1 << endl;
        a.pop();
        b.pop();
    }
}