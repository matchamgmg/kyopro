#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    string s;
    cin >> s;
    for(int i=2;i<=16;i=i+2){
        if(s.at(i-1)!='0'){
            cout << "No" << endl;
            return 0;
        }
    }
    cout << "Yes" << endl;
}