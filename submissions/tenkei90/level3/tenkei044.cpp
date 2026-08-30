#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,Q;
    cin >> N >> Q;
    deque<int> d;
    int n;
    rep(i,N){
        cin >> n;
        d.push_back(n);
    }
    int t,x,y;
    rep(i,Q){
        cin >> t >> x >> y;
        if(t==1){
            swap(d[x-1],d[y-1]);
        }else if(t==2){
            int m = d.back();
            d.pop_back();
            d.push_front(m);
        }else if(t==3){
            cout << d[x-1] << endl;
        }
    }
}