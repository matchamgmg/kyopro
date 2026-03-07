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
using vvll = vector<vll>;

int main()
{
    char a,b,c;
    cin >> a >> b >> c;
    if(a=='>'&&c=='>'&&b=='>'||a=='<'&&c=='<'&&b=='<'){
        cout << "B" << endl;
    }else if(a=='>'&&b=='>'&&c=='<'||a=='<'&&b=='<'&&c=='>'){
        cout << "C" << endl;
    }else{
        cout << "A" << endl;
    }
}