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
    string s;
    cin >> s;
    vi keyboard(26);
    int cost = 0;
    rep(i,0,26){
        keyboard[s[i]-'A'] = i;
    }
    rep(i,0,25){
        cost += abs(keyboard[i+1]-keyboard[i]);
    }
    cout << cost << endl;
}