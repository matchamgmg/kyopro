#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    string s;
    cin >> N;
    int cnt = 0;
    rep(i,N){
        cin >> s;
        if(s == "Takahashi") cnt++;
    }
    cout << cnt << endl;
}