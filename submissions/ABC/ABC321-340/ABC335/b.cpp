#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int N;
    cin >> N;
    int i=0,j=0,k=0;
    for(i=0;i<=N;i++){
        for(j=0;i+j<=N;j++){
            for(k=0;i+j+k<=N;k++){
                cout << i << " " << j << " " << k << endl;
            }
        }
    }
}