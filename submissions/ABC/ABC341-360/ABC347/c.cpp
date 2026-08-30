#include <bits/stdc++.h>
using namespace std;

int main()
{
    int N, A, B;
    cin >> N >> A >> B;
    vector<int> D(N);
    for (int i = 0; i < N; i++)
    {
        cin >> D.at(i);
        D.at(i) %= (A+B);
    }
    sort(D.begin(),D.end());
    D.push_back(D.at(0)+A+B);
    for(int i=0;i<D.size()-1;i++){
        if(D.at(i+1)-D.at(i)>B){
            cout << "Yes" << endl;
            return 0;
        }
    }
    cout << "No" << endl;
}