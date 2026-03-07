#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll cross(pair<ll,ll> &a, pair<ll,ll> &b){
    return a.first*b.second - a.second*b.first;
}

bool up(pair<ll,ll> &a){
    return a.second > 0 || (a.second == 0 && a.first >= 0);
}


// return argument(a) < argument(b)
bool comp(pair<ll,ll> &a, pair<ll,ll> &b){
    int aup = up(a);
    int bup = up(b);
    if(aup != bup) return aup > bup;
    return cross(a, b) > 0;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << numeric_limits<double>::epsilon() << endl;
    cout << numeric_limits<long double>::epsilon() << endl;

    int N;
    cin >> N;
    vector<pair<ll,ll>> P(N);
    for(int i=0; i<N; ++i){
        cin >> P[i].first >> P[i].second;
    }

    sort(P.begin(), P.end(), comp);
    
    for(int i=0; i<N; ++i){
        cout << P[i].first << ' ' << P[i].second << endl;
    }
}