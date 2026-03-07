#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int A,B,D;
    cin >> A >> B >> D;
    while(A<=B){
        cout << A << " ";
        A += D;
    }
    cout << endl;
}