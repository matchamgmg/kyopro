#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int k,G,M;
    cin>>k>>G>>M;
    int g=0,m=0;
    rep(i,k){
        if(g==G){
            g=0;
        }else if(m==0){
            m=M;
        }else{
            if(G-g>=m){
                g+=m;
                m=0;
            }else{
                m-=G-g;
                g=G;
            }
        }
    }
    cout << g << " " << m << endl;
}