#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n;
    cin>>n;
    string s;
    cin >> s;
    rep(i,n-1){
        if(s.at(i)=='a'&&s.at(i+1)=='b'){
            cout << "Yes" << endl;
            return 0;
        }
        if(s.at(i)=='b'&&s.at(i+1)=='a'){
            cout << "Yes" << endl;
            return 0;
        }
    }
    cout << "No" <<endl;
}