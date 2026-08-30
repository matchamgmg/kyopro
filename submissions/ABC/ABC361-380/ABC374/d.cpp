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

struct point
{
    ll x,y;
};

using edge = pair<point,point>;

long double calc_dist(point g,point s){
    ll sq_d = (g.x-s.x)*(g.x-s.x) + (g.y-s.y)*(g.y-s.y);
    long double d = sqrt((long double)sq_d);
    return d;
}

int main()
{
    int N,S,T;
    cin >> N >> S >> T;
    vector<edge> ABCD(N);
    vi idx(N);
    rep(i,0,N){
        point a,b;
        cin >> a.x >> a.y >> b.x >> b.y;
        ABCD[i] = {a,b};
        idx[i] = i;
    }
    long double min_time = 1000000000000;
    do{
        for(int i=0;i<(1<<N);i++){
            vector<int> bits(N);
            for(int j=0;j<N;j++){
                int div = (1<<j);
                bits.at(j) = (i/div)%2;
            }
            point start;
            start.x = 0;
            start.y = 0;
            point goal;
            goal.x = 0;
            goal.y = 0;
            long double d_S = 0;
            long double d_T = 0;
            rep(j,0,N){
                if(bits[j]==1){
                    start = ABCD[idx[j]].first;
                }else{
                    start = ABCD[idx[j]].second;
                }
                d_S += calc_dist(goal,start);
                if(bits[j]==1){
                    goal = ABCD[idx[j]].second;
                }else{
                    goal = ABCD[idx[j]].first;
                }
                d_T += calc_dist(start,goal);
                //cout << bits[j] << " ";
            }
            //cout << endl;
            //cout << d_S << endl;
            //cout << d_T << endl;
            //cout << endl;
            long double time = d_S/S + d_T/T;
            min_time = min(min_time,time);
        }
    }while(next_permutation(ALL(idx)));
    printf("%.12Lf\n",min_time);
}