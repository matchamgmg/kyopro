#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    deque<int> deck;
    int q;
    cin >> q;
    int t,x;
    rep(i,q){
        cin >> t >> x;
        if(t==1){
            deck.push_front(x);
        }else if(t==2){
            deck.push_back(x);
        }else if(t==3){
            cout << deck.at(x-1) << endl;
        }
    }
}