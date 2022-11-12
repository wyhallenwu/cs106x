#include <iostream>
#include <map>
#include <string>
using namespace std;

static string generateStory(const string &storyTemplate,
                            const map<string, string> &data) {
  int index = 0;
  string result;
  while (index < storyTemplate.size()) {
    if (storyTemplate[index] == '{') {
      string token;
      int start = index++;
      while (storyTemplate[index] != '}') {
        token += storyTemplate[index++];
      }
      index++;
      result += data.at(token);
    } else {
      result += storyTemplate[index++];
    }
  }
  return result;
}
int main() {
  string input = "{actor} gave {restaurant} a {rating}-star review.";
  map<string, string> m;
  m["actor"] = "Mike Vernal";
  m["restaurant"] = "The French Laundry";
  m["rating"] = "5";
  string result = generateStory(input, m);
  cout << result << endl;
  return 0;
}
