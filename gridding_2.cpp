#include <stdbool.h>
#include <time.h>
#include <uC++.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

_Task MyTask2 {
 public:
  MyTask2(int id, int N, float deltaX, FileReader& reader, FileMonitor& monitor,
          vector<float>& fr, vector<float>& fi, vector<float>& wt)
      : id(id),
        N(N),
        deltaX(deltaX),
        reader(reader),
        monitor(monitor),
        fr(fr),
        fi(fi),
        wt(wt) {}
  ~MyTask2() {}
  int id;
  int N;
  float deltaX;
  FileReader& reader;
  FileMonitor& monitor;
  vector<float>& fr;
  vector<float>& fi;
  vector<float>& wt;
  vector<string> vc;

 private:
  float deg_to_rad(float deg) { return deg * M_PI / (180 * 3600); }
  vector<float> str_to_vec(string str) {
    stringstream ss(str);
    vector<float> vis;
    while (ss.good()) {
      string substr;
      getline(ss, substr, ',');
      vis.push_back(stof(substr));
    }
    return vis;
  }
};
// _Mutex WriteMatrix{
//   private : public : WriteMatrix(){} ~WriteMatrix(){}

// };
void uMain::main() {
  MyTask2* tasks_2[t];                     // Array of tasks
  FileReader reader2(input_file_name, c);  // Comonitor object creation
  FileMonitor monitor;

  vector<float> fr2(N * N);
  vector<float> fi2(N * N);
  vector<float> wt2(N * N);

  for (int i = 0; i < t; i++) {
    tasks_2[i] = new MyTask2(i, N, deltaX, reader, monitor, fr2, fi2,
                             wt2);  // Allocation
  }
  for (int i = 0; i < t; i++) {
    delete tasks_2[i];  // Task finalization
  }
  for (int k = 0; k < N * N; k++) {  // Weight normalization
    if (wt2[k] != 0) {
      fr2[k] = fr2[k] / wt2[k];
      fi2[k] = fi2[k] / wt2[k];
    }
  }
  string r_file_name = output_file_name + "2r.raw";
  string i_file_name = output_file_name + "2i.raw";

  reader.write_file(fr2, N * N, r_file_name);
  reader.write_file(fi2, N * N, i_file_name);
}