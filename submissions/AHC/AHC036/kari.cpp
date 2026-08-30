#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

void a(vector<int> A){
    A[0]++;
}

int main()
{
    vector<int> A(3,1);
    a(A);
    rep(i,A.size()){
        cout << A[i] << " ";
    }
    cout << endl;
}