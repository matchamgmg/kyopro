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

int pow(int x,int n){
    int ret = 1;
    while(n > 0){
      if(n & 1){  //nの最下位ビットが1のとき
        ret *= x;
      }
      x *= x;     //x^(2^i)を作っている
      n >>= 1;
    }
    return ret;
  }

int main()
{
    int M;
    cin >> M;
    int m = M;
    for(int a=10;a>=0;a--){
        vi ans(20,-1);
        int size_a = 0;
        rep(i,0,20){
            int j=a;
            while(pow(3,j)>m&&j>0){
                j--;
            }
            if(pow(3,j)<=m){
                m -= pow(3,j);
                ans[size_a] = j;
                size_a++;
                //cout << j << endl;
            }
            if(m==0){
                cout << size_a << endl;
                rep(l,0,size_a){
                    cout << ans[l] << " ";
                }
                cout << endl;
                return 0;
            }
        }
    }
}