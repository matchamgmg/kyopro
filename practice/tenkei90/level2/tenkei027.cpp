#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    map<string,int> m;
    string s;
    rep(i,N){
        cin >> s;
        if(m[s]==0){
            cout << i+1 << endl;
        }
        m[s]++;
    }
}