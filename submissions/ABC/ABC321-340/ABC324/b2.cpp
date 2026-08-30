#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)
using ll =long long;

int main()
{
    ll n;
    cin >> n;
    while(n%2==0){
        n /= 2;
    }
    while(n%3==0){
        n /= 3;
    }
    cout << (n==1?"Yes":"No") << endl;
}