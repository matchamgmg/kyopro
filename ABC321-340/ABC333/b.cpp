#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    string s1,s2;
    cin >> s1 >> s2;
    int l1,l2;
    ((s1.at(0)-s1.at(1)+5)%5==1||(s1.at(0)-s1.at(1)+5)%5==4)?l1=1:l1=2;
    ((s2.at(0)-s2.at(1)+5)%5==1||(s2.at(0)-s2.at(1)+5)%5==4)?l2=1:l2=2;
    cout << (l1==l2?"Yes":"No") << endl;
}