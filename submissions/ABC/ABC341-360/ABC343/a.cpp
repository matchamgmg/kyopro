#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int A,B;
    cin >> A>>B;
    rep(i,10){
        if(i!=A+B){
            cout << i << endl;
            return 0;
        }
    }
}