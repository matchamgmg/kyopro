#include <bits/stdc++.h>
using namespace std;
int main()
{
    int N;
    cin >> N;
    vector<int> A(N), B(N);
    for (int i = 0; i < N; i++)
    {
        cin >> A.at(i) >> B.at(i);
    }
    int64_t max_height = 0;
    int64_t sum_height =0;
    for(int i=0;i<N;i++){
        sum_height+=A.at(i);
    }
    for(int i=0;i<N;i++){
        if(max_height<(sum_height-A.at(i)+B.at(i))){
            max_height=sum_height-A.at(i)+B.at(i);
        }
    }
    cout << max_height << endl;
}