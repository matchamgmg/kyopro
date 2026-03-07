#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    vector<vector<int>> A(9,vector<int>(9)),B(9,vector<int>(9));
    int a;
    rep(i,9){
        rep(j,9){
            cin >> a;
            A.at(i).at(j) = a;
            B.at(j).at(i) = a;
        }
    }
    vector<bool> tate(9,false),yoko(9,false),area(9,false);
    vector<set<int>> s(9,set<int>());
    rep(i,9){
        if(size(set<int>(A.at(i).begin(),A.at(i).end()))==9){
            yoko.at(i) = true;
        }
        if(size(set<int>(B.at(i).begin(),B.at(i).end()))==9){
            tate.at(i) = true;
        }
        rep(j,9){
            s.at(3*(i/3)+(j/3)).insert(A.at(i).at(j));
        }
    }
    rep(i,9){
        if(s.at(i).size()==9){
            area.at(i) = true;
        }
    }
    bool ans = true;
    rep(i,9){
        ans &= (tate.at(i) & yoko.at(i) & area.at(i));
    }
    cout << (ans?"Yes":"No") << endl;
}