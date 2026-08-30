#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    string s;
    cin >> s;
    reverse(s.begin(),s.end());
    rep(i,s.size()){
        if(s.at(i)=='.'){
            string ss = s.substr(0,i);
            reverse(ss.begin(),ss.end());
            cout << ss << endl;
            return 0;
        }
    }
}