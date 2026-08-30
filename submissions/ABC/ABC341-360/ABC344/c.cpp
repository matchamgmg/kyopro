#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int N, M, L, Q;
    vector<int> A, B, C, X;
    int x;
    cin >> N;
    rep(i, N)
    {
        cin >> x;
        A.push_back(x);
    }
    cin >> M;
    rep(i, M)
    {
        cin >> x;
        B.push_back(x);
    }
    cin >> L;
    rep(i, L)
    {
        cin >> x;
        C.push_back(x);
    }
    cin >> Q;
    rep(i, Q)
    {
        cin >> x;
        X.push_back(x);
    }
    set<int> S;
    rep(i , N)
    {
        rep(j, M)
        {
            rep(k, L)
            {
                S.insert(A.at(i) + B.at(j) + C.at(k));
            }
        }
    }
    rep(i, Q)
    {
        cout << (S.count(X.at(i)) ? "Yes" : "No") << endl;
    }
}