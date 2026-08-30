#include <bits/stdc++.h>
using namespace std;
int main(){
    string s;
    cin >> s;
    set<string> ss;
    for(int i=0;i<s.size();i++){
        for(int j=1;j<=s.size();j++){
           ss.insert(s.substr(i,j));
           //std::set 重複しない
        }
    }
    cout << ss.size() << endl;
}