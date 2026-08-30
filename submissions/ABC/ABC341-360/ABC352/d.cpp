#include <bits/stdc++.h>
using namespace std;
int main()
{
    int N, K;
    cin >> N >> K;
    pair<int, int> p;
    vector<int> A(N);
    int l;
    for (int i = 0; i < N; i++)
    {
        cin >> l;
        A.at(l - 1) = i + 1;
    }

    vector<int> index_gap(N - K + 1);
    int max_index;
    int min_index;
    int tmp;
    int min_index_gap = N;
    for (int i = 0; i < N - K + 1; i++)
    {
        max_index = 0;
        min_index = N;
        for (int j = 0; j < K; j++)
        {
            tmp = A.at(i + j);
            if (max_index < tmp)
            {
                max_index = tmp;
            }
            if (min_index > tmp)
            {
                min_index = tmp;
            }
        }
        index_gap.at(i) = max_index - min_index;
        if (min_index_gap > index_gap.at(i))
        {
            min_index_gap = index_gap.at(i);
        }
    }

    cout << min_index_gap << endl;
}