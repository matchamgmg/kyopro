#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    string s;
    cin >> s;
    if(!isupper(s.at(0))){
        cout << "No" << endl;
        return 0;
    }
    rep(i,s.size()-1){
        if(isupper(s.at(i+1))){
            cout << "No" << endl;
            return 0;
        }
    }
    cout << "Yes" << endl;
}