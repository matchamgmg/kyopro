#include <bits/stdc++.h>
using namespace std;

int main(){
    int i=0;
    auto f = [&i](){return i++;};
    for(int j=0;j<3;j++){
        cout << f() << endl;
    }
}