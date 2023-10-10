#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
int main() {
  int nrows = 100;  // rows and columns
  int ncols = 100;  // rows and columns
  int initial_value = 0;
  std::vector<std::vector<double>> fr;
  fr.resize(nrows, std::vector<double>(ncols, initial_value));
  cout << fr.size() << " x " << fr[0].size() << " x " << fr[0][0] << endl;
  return 1;
}
