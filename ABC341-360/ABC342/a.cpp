#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    string s;
    cin >> s;
    for(int i=1;i<s.size()-1;i++){
        if(s.at(0)!=s.at(i)){
            cout << (s.at(i+1)!=s.at(0)?1:i+1) << endl;
            return 0;
        }else if(s.at(i)!=s.at(i+1)){
            cout << i+2 << endl;
            return 0;
        }
    }
}