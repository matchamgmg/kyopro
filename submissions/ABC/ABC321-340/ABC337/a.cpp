#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int N;
    cin >> N;
    int a,b;
    int suma=0,sumb=0;
    rep(i,N){
        cin >> a >> b;
        suma+=a;
        sumb+=b;
    }
    cout << (suma>sumb?"Takahashi":suma<sumb?"Aoki":"Draw") << endl;
}