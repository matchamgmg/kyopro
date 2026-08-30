#include <bits/stdc++.h>
using namespace std;
int main(){
    string s;
    cin >> s;
    vector<string> ss;
    string t;
    bool a=true;
    for(int i=0;i<s.size();i++){
        for(int j=1;j<=s.size();j++){
            t=s.substr(i,j);
            a=true;
            for(int k=0;k<ss.size();k++){
                if(ss.at(k)==t){
                    a=false;
                }
            }
            if(a){
                ss.push_back(t);
            }
        }
    }
    cout << ss.size() << endl;
}