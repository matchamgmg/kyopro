#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    string S,T;
    cin >> S >> T;
    int N = S.size();
    for(int w=1;w<N;w++){
        for(int c=0;c<w;c++){
            string s = "";
            int n = N;
            int i = 0;
            while(n>c){
                s.push_back(S[w*i+c]);
                i++;
                n -= w;
            }
            if(s==T){
                cout << "Yes" << endl;
                return 0;
            }
        }
    }
    cout << "No" << endl;
}