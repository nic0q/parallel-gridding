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
const double speed_of_light = 2.998 * 1e8;
// n° lineas: 3.196.373
_Cormonitor FileReader {
 public:
  FileReader(string & filename, int chunkSize) : chunk(chunkSize) {
    file.open(filename.c_str());
  }
  ~FileReader() { file.close(); }
  void write_file(vector<double> vc, int size, string& file) {
    FILE* outputFile = fopen(file.c_str(), "wb");
    if (!outputFile) {
      cerr << "Error al abrir el archivo: " << file << endl;
    }
    if (fwrite(vc.data(), sizeof(double), size, outputFile) == size) {
      cout << "All elements were written successfully in " << file << endl;
    } else {
      cout << "There was an error while writing the elements in " << file
           << endl;
    }
    fclose(outputFile);
  }
  vector<string> next() {
    resume();
    return vtr;
  }
  int n_line = 0;
  bool is_done() { return file.eof(); }

  void clean_vtr() { vtr.clear(); }

  vector<string> vtr;

 private:
  ifstream file;
  int chunk;

  void main() {
    string line;
    for (;;) {
      for (int i = 0; i < chunk; i++) {  // Reading X lines of chunk
        if (!getline(file, line)) {
          break;
        } else {
          vtr.push_back(line);
          n_line += 1;
        }
      }
      suspend();
    }
  }
};

_Task MyTask {
 public:
  MyTask(int id, int N, double deltaX, FileReader& reader)
      : id(id), N(N), deltaX(deltaX), reader(reader) {
    fr.resize(N * N, 0);
    fi.resize(N * N, 0);
    wt.resize(N * N, 0);
  }

  int id;
  int N;
  int arg = 3;
  double deltaX;

  FileReader& reader;
  bool isDone = false;
  vector<string> vc;
  double arc_sec_to_rad(double deg) { return deg * M_PI / (180 * 3600); }
  vector<double> str_to_vec(string str) {
    stringstream ss(str);
    vector<double> vis;
    while (ss.good()) {
      string substr;
      getline(ss, substr, ',');
      vis.push_back(stod(substr));
    }
    return vis;
  }
  vector<double> get_fr() { return fr; }
  vector<double> get_fi() { return fi; }
  vector<double> get_wt() { return wt; }

 protected:
  vector<double> fr;
  vector<double> fi;
  vector<double> wt;
  void main() {
    cout << "Start Task(" << id << ")" << endl;
    while (!isDone) {
      if (reader.n_line % 10000 == 0) {
        cout << "Task:\t" << id << "\tLine:\t" << reader.n_line << endl;
      }
      isDone = reader.is_done();
      vc = reader.next();
      vector<double> vis;  // string vector
      for (int i = 0; i < vc.size(); i++) {
        vis = str_to_vec(vc[i]);  // double vector
        double uk, vk, vr, vi, wk, f, deltaU, deltaV, ik, jk;
        int spc;
        uk = vis[0];
        vk = vis[1];
        vr = vis[3];
        vi = vis[4];
        wk = vis[5];
        f = vis[6];
        spc = vis[7];
        deltaU = 1 / (N * arc_sec_to_rad(deltaX));  // to radians
        deltaU = deltaU * (f / speed_of_light);     // to wave longitude
        deltaV = deltaU;  // asuming deltav is equals to deltau
        ik = round((uk / deltaU) + (N / 2));
        jk = round((vk / deltaV) + (N / 2));
        // cout << "uk: " << uk << " vk: " << vk << " vr: " << vr << " vi: " <<
        // vi
        //      << " wk: " << wk << " | ik(" << ik << ") jk(" << jk << ")" <<
        //      endl;
        fr[ik * N + jk] = fr[ik * N + jk] + (wk * vr);
        fi[ik * N + jk] = fi[ik * N + jk] + (wk * vi);
        wt[ik * N + jk] = wt[ik * N + jk] + wk;
      }
      reader.vtr.clear();
    }
    cout << "End Task(" << id << ")" << endl;
  }
};
void uMain::main() {
  string input_file_name;
  string output_file_name;
  double deltaX = 0.0, tp = 0.0, time;
  unsigned t0, t1;
  int N = 0;
  int c = 0;
  int t = 0;
  int option;
  while ((option = getopt(argc, argv, "i:o:d:N:c:t:")) != -1) {
    switch (option) {
      case 'i':
        input_file_name = optarg;
        break;
      case 'o':
        output_file_name = optarg;
        break;
      case 'd':
        deltaX = stod(optarg);
        break;
      case 'N':
        N = stoi(optarg);
        break;
      case 'c':
        c = stoi(optarg);
        break;
      case 't':
        t = stoi(optarg);
        break;
      default:
        cerr << "Uso: " << argv[0]
             << " -i input -o output -d deltaX -N tamaño -c chunk -t "
                "tareas"
             << endl;
    }
  }
  cout << "Input File: " << input_file_name << endl;
  cout << "Output File: " << output_file_name << endl;
  cout << "DeltaX: " << deltaX << endl;
  cout << "Image Size: " << N << endl;
  cout << "Chunk Size: " << c << endl;
  cout << "Number of Tasks: " << t << endl;

  vector<double> fr(N * N);
  vector<double> fi(N * N);
  vector<double> wt(N * N);

  FileReader reader(input_file_name, c);  // Comonitor object creation
  MyTask** tasks = new MyTask*[t];        // Array of tasks

  for (int i = 0; i < t; i++) {
    tasks[i] = new MyTask(i, N, deltaX, reader);  // Allocation
  }
  for (int i = 0; i < t; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < N; k++) {
        fr[j * N + k] += tasks[i]->get_fr()[j * N + k];  // r
        fi[j * N + k] += tasks[i]->get_fi()[j * N + k];  // i
        wt[j * N + k] += tasks[i]->get_wt()[j * N + k];  // w
      }
    }
    delete tasks[i];
  }
  for (int j = 0; j < N; j++) {
    for (int k = 0; k < N; k++) {
      fr[j * N + k] = fr[j * N + k] / wt[j * N + k];  // r
      fi[j * N + k] = fi[j * N + k] / wt[j * N + k];  // i
    }
  }
  string r_file = output_file_name + "r.raw";
  string i_file = output_file_name + "i.raw";

  reader.write_file(fr, N * N, r_file);
  reader.write_file(fi, N * N, i_file);

  delete[] tasks;
}
