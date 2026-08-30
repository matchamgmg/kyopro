#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int H,W;
    cin >> H >> W;
    if(H==1||W==1){
        cout << H*W << endl;
    }else{
        cout << ((H-1)/2 + 1)*((W-1)/2 +1) << endl;
    }
}