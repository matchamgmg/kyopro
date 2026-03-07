#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using ll = long long;

int main()
{
    int K;
    cin >> K;

    vector<int> bits(10, 0); // 先頭から0,1,...,9
    vector<ll> answer;
    for (int i = 2; i < (1 << 10); i++)
    {
        rep(j, 10)
        {
            int div = 1 << j;
            bits.at(j) = (i / div) % 2;
        }

        ll ans = 0;
        int cnt = 0;
        
        rep(j, 10)
        {
            if (bits.at(j) == 1)
            {
                ll s = j;
                rep(k, cnt)
                {
                    s *= 10;
                }
                ans += s;
                cnt++;
            }
        }
        answer.push_back(ans);
        
    }
    
    sort(ALL(answer));
    cout << answer.at(K-1);
    
}