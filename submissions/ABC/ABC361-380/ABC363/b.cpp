#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,T,P;
    cin >> N >> T >> P;
    vector<int> L(N);
    rep(i,N){
        cin >> L[i];
    }
    sort(ALL(L));
    int lngP = L[N-P];
    if(lngP>=T){
        cout << 0 << endl;
        return 0;
    }
    cout << (T - lngP) << endl;
}