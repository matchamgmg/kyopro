#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int N;
    cin >> N;
    vector<int> A(N);
    int front;
    rep(i,N){
        cin >> A.at(i);
        if(A.at(i)<0){
            front = i;
        }
    }
    vector<int> B(N,-1);
    int j=0;
    rep(i,N){
        if(A.at(i)-1>=0){
            B.at(A.at(i)-1)=i+1;
        }
    }
    while(front>=0){
        cout << front + 1 << " ";
        front = B.at(front)-1;
    }
    cout << endl;
    // 1  2 3 4  5 i
    // 3 -1 2 1  4 A.at(i)
    // 4  3 1 5 -1 B.at(i)
}