#include <bits/stdc++.h>
#include <atcoder/all>
#define rep(i, s, n) for (int i = s; i < (n); i++)
#define ALL(a) a.begin(),a.end()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
using namespace std;
using namespace atcoder;
using ll = long long;
using pii = pair<int,int>;
using vi = vector<int>;
using vll = vector<ll>;
using vpii = vector<pii>;
using vvi = vector<vi>;
using vvl = vector<vll>;

int main()
{
    int l,r;
    cin >> l >> r;
    if((r==1&&l==1)||(r==0&&l==0)){
        cout << "Invalid" << endl;
    }else if(l==1){
        pyes;
    }else{
        pno;
    }
}