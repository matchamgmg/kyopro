#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<string> S(N);
    rep(i,N){
        cin >> S[i];
    }
    string last = S[0];
    for(int i=1;i<N-1;i++){
        if(last=="sweet"&&S[i]=="sweet"){
            cout << "No" << endl;
            return 0;
        }
        last = S[i];
    }
    cout << "Yes" << endl;
}