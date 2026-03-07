#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    string s;
    cin >> s;
    vector<char> abc = {'A','B','C'};
    int mode=0;
    rep(i,s.size()){
        if(s.at(i)!=abc.at(mode)){
            if(mode<2&&s.at(i)!=abc.at(mode+1)){
                if(mode<1){
                    mode += 2;
                }else{
                    cout << "No" << endl;
                    return 0;
                }
            }else if(mode<2){
                mode++;
            }else{
                cout << "No" << endl;
                return 0;
            }
        }
    }
    cout << "Yes" << endl;
}