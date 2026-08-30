#include<bits/stdc++.h>
using namespace std;

int main(){
  int N;
  cin >> N;
  string S;
  cin >> S;
  int i = 0;
  int ans;
  bool a = false, b = false, c = false;
  bool ok = false;
  while(i<N && !ok){
    a |= (S[i] == 'A');
    b |= (S[i] == 'B');
    c |= (S[i] == 'C');
    if(a && b && c){
      ok = true;
      ans = i+1;
      break;
    }
    ++i;
  }
  cout << ans << endl;
}