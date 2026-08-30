#include <bits/stdc++.h>
using namespace std;
int main()
{
    int N;
    cin >> N;
    vector<int> A(N), B(N);
    for (int i = 0; i < N; i++)
    {
        cin >> A.at(i) >> B.at(i);
    }
    auto farthest_point = [&](int i)
    {
        int xi = A.at(i), yi = B.at(i);
        int xj, yj;
        vector<int> distance(N);
        for (int j = 0; j < N; j++)
        {
            xj = A.at(j);
            yj = B.at(j);
            distance.at(j) = (xi - xj) * (xi - xj) + (yi - yj) * (yi - yj);
        }
        int max_distance = 0;
        int n;
        for (int j = 0; j < N; j++)
        {
            if (max_distance < distance.at(j))
            {
                max_distance = distance.at(j);
                n = j;
            }
        }
        return n;
    };
    for (int i = 0; i < N; i++)
    {
        cout << farthest_point(i) + 1 << endl;
    }
}