#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n;
    cin>>n;
    for(int i=n;i<1000;i++){
        int a = i/100;
        int b = (i-a*100)/10;
        int c = (i-a*100-b*10);
        if(a*b==c){
        cout << a*100+b*10+c << endl;
        return 0;
        }
    }
}