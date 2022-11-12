#include <iostream>
#include <queue>
#include <stack>
using namespace std;

static int getTopswopNumber(stack<int> s) {
  int count = 0;
  while (true) {
    int top = s.top();
    if (top == 1)
      return count;
    queue<int> q;
    for (int i = 0; i < top; i++) {
      q.push(s.top());
      s.pop();
    }
    while (!q.empty()) {
      s.push(q.front());
      q.pop();
    }
    count++;
  }
}
int main() {
  stack<int> s;
  s.emplace(2);
  s.emplace(5);
  s.emplace(1);
  s.emplace(3);
  s.emplace(4);
  int count = getTopswopNumber(s);
  cout << count << endl;
  return 0;
}
