#include <vector>
#include <algorithm>

using namespace std;
using ll = long long;
#define rep(i, s, n) for (int i = s; i < (int)n; i++)

int main(){
    int N,M;
    cin >> N >> M;
    vector<ll> B(N),W(M);
    rep(i,0,N){
        cin >> B[i];
    }
    rep(i,0,M){
        cin >> W[i];
    }
    vector<ll> rui_B(N+1,0)
}