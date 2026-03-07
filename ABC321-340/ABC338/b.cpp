#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    string S;
    cin >> S;
    vector<int> f(26,0);
    rep(i,S.size()){
        f.at(S.at(i)-'a')++;
    }

    int index=0;
    int maxf=0;
    rep(i,26){
        if(maxf<f.at(i)){
            maxf=f.at(i);
            index=i;
        }
    }
    cout << (char)(index +'a') << endl;
}