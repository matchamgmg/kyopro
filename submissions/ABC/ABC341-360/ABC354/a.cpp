#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int H;
    cin >> H;
    int height=0;
    int day=0;
    while(height<=H){
        height += 1 << day;
        day++;
    }
    cout << day << endl;
    day = exp2(day);
}