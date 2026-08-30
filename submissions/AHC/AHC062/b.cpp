#include <bits/stdc++.h>
using namespace std;
int main()
{
    int N;
    cin >> N;
    vector<vector<int>> A(N, vector<int>(N));
    long long SA = 0;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            cin >> A[i][j];
            SA += A[i][j];
        }
    }
    SA *= (N * N - 1);
    long long score_max = 0;
    vector<pair<int, int>> ans(N * N, {0, 0});
    long long score = 0;
    vector<pair<int, int>> ans1(N * N);
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            int jj = ((i & 1) ? j : N - 1 - j);
            ans1[i * N + j] = {i, j};
            score += (i * N + j) * (long long)A[i][j];
        }
    }
    if (score < SA - score)
        reverse(ans1.begin(), ans1.end());
    if (score > score_max)
    {
        swap(ans, ans1);
        score_max = score;
    }
    score = 0;
    for (int j = 0; j < N; ++j)
    {
        for (int i = 0; i < N; ++i)
        {
            int ii = ((j & 1) ? i : N - 1 - i);
            ans1[j * N + i] = {j, i};
            score += (j * N + i) * (long long)A[j][i];
        }
    }
    if (score < SA - score)
        reverse(ans1.begin(), ans1.end());
    if (score > score_max)
    {
        swap(ans, ans1);
        score_max = score;
    }
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            cout << ans[i * N + j].first << ' ' << ans[i * N + j].second << '\n';
        }
    }
}