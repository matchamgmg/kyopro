#include<bits/stdc++.h>
using namespace std;

struct nekoset {
    private:
    set<pair<int,int>> s;

    public:
    nekoset(){
        s.emplace(INT_MIN,INT_MIN);
        s.emplace(INT_MAX,INT_MAX);
    }

    bool contains(int x) const {
        auto it = prev(s.lower_bound(make_pair(x+1,x+1)));
        auto [l,u] = *it;
        return l<=x && x<=u;
    }

    bool insert(int x){
        auto nit = s.lower_bound(make_pair(x+1,x+1));
        auto it = prev(nit);
        auto [l,u] = *it;
        auto [nl,nu] = *nit;
        if(l<=x && x<=u) return false;
        if(u==x-1){
            if(nl==x+1){
                s.erase(it);
                s.erase(nit);
                s.emplace(l,nu);
            }else{
                s.erase(it);
                s.emplace(l,x);
            }
        }else{
            if(nl==x+1){
                s.erase(nit);
                s.emplace(x,nu);
            }else{
                s.emplace(x,x);
            }
        }
        return true;
    }

    bool erase(int x){
        auto it = prev(s.lower_bound(make_pair(x+1,x+1)));
        auto [l,u] = *it;
        if(!(l<=x && x<=u)) return false;
        if(x==l){
            s.erase(it);
            s.emplace(l+1,u);
        }else if(x==u){
            s.erase(it);
            s.emplace(l,u-1);
        }else{
            s.erase(it);
            s.emplace(l,x-1);
            s.emplace(x+1,u);
        }
        return true;
    }

    int mex(int x = 0) const {
        auto [l,u] = *prev(s.lower_bound(make_pair(x+1,x+1)));
        if(l<=x && x<=u){
            return u+1;
        }else{
            return x;
        }
    }
};