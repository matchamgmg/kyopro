#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int N;
    cin >> N;
    int h0;
    cin >> h0;
    int hi;
    bool a = false;
    rep(i,N-1){
        cin >> hi;
        if(hi>h0){
            cout << i+2 << " "<< endl;
            a=true;
            break;
        }
    }
    if(!a){
        cout << -1 <<endl;
    }
}