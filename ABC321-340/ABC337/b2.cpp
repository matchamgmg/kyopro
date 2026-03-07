#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    string s;
    cin >> s;
    if(regex_match(s,regex("A*B*C*"))){
        cout << "Yes" << endl;
    }else{
        cout << "No" << endl;
    }
}