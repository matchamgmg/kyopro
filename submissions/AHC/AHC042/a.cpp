#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
//using mint = modint1000000007;
using vi = vector<int>;
using vll = vector<ll>;
using vld = vector<ld>;
using vmint = vector<mint>;
using vvi = vector<vi>;
using vvll = vector<vll>;
using vvld = vector<vld>;
using vvmint = vector<vmint>;
using vvvi = vector<vvi>;
using vvvll = vector<vvll>;
using vvvld = vector<vvld>;
using vvvmint = vector<vvmint>;
using vb = vector<bool>;
using vvb = vector<vb>;
template<class T> using pq = priority_queue<T, vector<T>>;//大きい順
template<class T> using pq_g = priority_queue<T, vector<T>, greater<T>>;//小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

void move(char c, int n, int r){
    rep(i,0,n){
        cout << c << ' ' << r << endl;
    }
    return;
}

int main()
{
    int N;
    cin >> N;
    vector<string> C(N);
    rep(i,0,N){
        cin >> C[i];
    }
    
    rep(i,0,N){
        vi OFO(3,-1);
        int OFO_cnt = 0;
        bool check = true;
        rep(j,0,N){
            if(C[i][j]=='x'){
                if(OFO_cnt==1){
                    OFO_cnt++;
                    OFO[OFO_cnt] = j;
                }else if(OFO_cnt==0){
                    OFO[OFO_cnt] = j;
                }else if(OFO_cnt==2){
                    OFO[OFO_cnt] = j;
                }else{
                    check = false;
                }
            }
            if(C[i][j]=='o'){
                if(OFO_cnt==0){
                    OFO_cnt++;
                    OFO[OFO_cnt] = j;
                }else if(OFO_cnt==1){
                    OFO[OFO_cnt] = j;
                }else{
                    check = false;
                }
            }
        }
        if(check){
            // v_cout(OFO);
            if(OFO[0]==-1){
                if(OFO[1]==-1){//none
                    continue;
                }else if(OFO[2]==-1){//only F
                    continue;
                }else{//to right
                    move('R',N-OFO[1]-1,i);
                    move('L',N-OFO[1]-1,i);
                }
            }else if(OFO[1]==-1){//only O
                move('L',OFO[0]+1,i);
                move('R',OFO[0]+1,i);
            }else if(OFO[2]==-1){//to lef
                move('L',OFO[0]+1,i);
                move('R',OFO[0]+1,i);
            }else{//left ans right
                move('L',OFO[0]+1,i);
                move('R',OFO[0]-OFO[1]+N,i);
                move('L',N-OFO[1]-1,i);
            }
        }
    }

    rep(j,0,N){
        vi OFO(3,-1);
        int OFO_cnt = 0;
        bool check = true;
        rep(i,0,N){
            if(C[i][j]=='x'){
                if(OFO_cnt==1){
                    OFO_cnt++;
                    OFO[OFO_cnt] = i;
                }else if(OFO_cnt==0){
                    OFO[OFO_cnt] = i;
                }else if(OFO_cnt==2){
                    OFO[OFO_cnt] = i;
                }else{
                    check = false;
                }
            }
            if(C[i][j]=='o'){
                if(OFO_cnt==0){
                    OFO_cnt++;
                    OFO[OFO_cnt] = i;
                }else if(OFO_cnt==1){
                    OFO[OFO_cnt] = i;
                }else{
                    check = false;
                }
            }
        }
        if(check){
            // v_cout(OFO);
            if(OFO[0]==-1){
                if(OFO[1]==-1){//none
                    continue;
                }else if(OFO[2]==-1){//only F
                    continue;
                }else{//to right
                    move('D',N-OFO[1]-1,j);
                    move('U',N-OFO[1]-1,j);
                }
            }else if(OFO[1]==-1){//only O
                move('U',OFO[0]+1,j);
                move('D',OFO[0]+1,j);
            }else if(OFO[2]==-1){//to lef
                move('U',OFO[0]+1,j);
                move('D',OFO[0]+1,j);
            }else{//left ans right
                move('U',OFO[0]+1,j);
                move('D',OFO[0]-OFO[1]+N,j);
                move('U',N-OFO[1]-1,j);
            }
        }
    }
}