#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
  int N;
  cin >> N;
  vector<string> A(N), B(N);
  for (auto& s : A) cin >> s;
  for (auto& s : B) cin >> s;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (A[i][j] != B[i][j]) {
        cout << i + 1 << " " << j + 1 << endl;
        return 0;
      }
    }
  }
}
