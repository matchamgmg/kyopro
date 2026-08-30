#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n;
    string s;
    cin >> n >> s;
    vector<int> a(26,0);
    int max_len;
    int len;
    rep(i,26){
        max_len = 0;
        len = 0;
        rep(j,n){
            if(s.at(j)=='a'+i){
                len++;
                max_len = max(max_len,len);
            }else{
                max_len = max(max_len,len);
                len = 0;
            }
        }
        a.at(i) = max_len;
    }
    cout << accumulate(a.begin(),a.end(),0) << endl;
}