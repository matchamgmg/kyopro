#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int N;
    cin >> N;
    vector<int> Q(N),A(N),B(N);
    rep(i,N){
        cin >> Q.at(i);
    }
    rep(i,N){
        cin >> A.at(i);
    }
    rep(i,N){
        cin >> B.at(i);
    }
    set<int> mn;
    int m;
    int n;
    bool flag = true;
    for(m=0;flag;m++){
        n=INT_MAX;
        rep(i,N){
            if(Q.at(i)-m*A.at(i)<0){
                flag=false;
                break;
            }
            if(B.at(i)==0){
                continue;
            }
            n=min(n,(Q.at(i)-m*A.at(i))/B.at(i));
        }
        if(flag){
            mn.insert(m+n);
        }
    }
    cout << *mn.rbegin() << endl;
}