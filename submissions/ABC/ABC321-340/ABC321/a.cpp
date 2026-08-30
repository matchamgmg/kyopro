#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    string s;
    cin>>s;
    rep(i,s.size()-1){
        if(s.at(i)<=s.at(i+1)){
            cout << "No" << endl;
            return 0;
        }
    }
    cout << "Yes" << endl;
}