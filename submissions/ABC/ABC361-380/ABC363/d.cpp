#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    ll N;
    cin >> N;
    if(N<=10){
        cout << (N-1) << endl;
        return 0;
    }
    ll n = N;
    ll head;
    ll keta = 0;
    ll div = 1;
    while(n>0){
        n/=10;
        keta++;
        div*=10;
    }
    div /= 100;
    head = N/div;

    vector<int> kaibun(keta*2-3,0);
    if(11<=head&&head<20) kaibun.push_back(0);

    
}