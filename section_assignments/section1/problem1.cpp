#include <iostream>
#include <vector>
using namespace std;

int sum_last_four(const vector<int> &nums, int k) {
  if (nums.size() < k) {
    cout << "vector length is length is less than" << k << endl;
  }
  int sum{0};
  for (int i = nums.size() - 1; i >= nums.size() - k; --i) {
    sum += nums.at(i);
  }
  return sum;
}

void print_vector(const vector<int> &nums) {
  for (auto &num : nums) {
    cout << num << " ";
  }
  cout << endl;
}

static bool isKeith(int n) {
  vector<int> nums;
  int origin = n;
  int sum{0};
  int length{0};
  // initilize nums
  while (n != 0) {
    sum += n % 10;
    length++;
    nums.insert(nums.begin(), n % 10);
    n /= 10;
  }
  nums.push_back(sum);
  // is keith
  while (sum < origin) {
    sum = sum_last_four(nums, length);
    nums.push_back(sum);
    if (sum == origin) {
      print_vector(nums);
      return true;
    }
  }
  return false;
}

int main() {
  cout << "please input your number: " << endl;
  int n;
  cin >> n;
  if (!isKeith(n)) {
    cout << n << " is not keith number" << endl;
  }
  return 0;
}
