#include<bits/stdc++.h>
using namespace std;

#define rep(i,s,n) for(int i=(int)s; i<(int)n; ++i)

int main(){
    int N;
    cin >> N;
    int seed;
    cin >> seed;
    mt19937 mt(seed);
    cout << N << endl;
    rep(i,0,N+1){
        cout << mt() % 1000000000 << " \n"[i==N];
    }
    rep(i,0,N){
        cout << mt() % 1000000000 << " \n"[i==N-1];
    }
}