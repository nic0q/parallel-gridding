#include <stdbool.h>
#include <uC++.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
const double speed_of_light = 2.998 * 1e8;

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
      cout << "All elements were written successfully" << endl;
    } else {
      cout << "There was an error while writing the elements" << endl;
    }
    fclose(outputFile);
  }
  vector<string> next() {
    resume();
    return vtr;
  }

  bool is_done() { return file.eof(); }

  void clean_vtr() { vtr.clear(); }

  vector<string> vtr;

 private:
  ifstream file;
  int chunk;
  int n_line = 0;

  void main() {
    string line;
    while (true) {
      suspend();
      for (int i = 0; i < chunk; i++) {  // Reading X lines of chunk
        if (!getline(file, line)) {
          break;
        } else {
          vtr.push_back(line);
          cout << "read " << n_line << endl;
          n_line += 1;
        }
      }
    }
  }
};

_Task MyTask {
 public:
  MyTask(int id, int N, double deltaX, FileReader& reader)
      : id(id), N(N), deltaX(deltaX), reader(reader) {
    fr.resize(N * N, 2);
    fi.resize(N * N, 0);
    wt.resize(N * N, 0);
  }

  int id;
  int a = 3;
  int N;
  double deltaX;
  vector<double> fr;
  vector<double> fi;
  vector<double> wt;
  FileReader& reader;
  vector<string> vc;

 private:
  double deg_to_rad(double deg) { return deg * M_PI / (180 * 3600); }

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

  void main() {
    cout << "Start(" << id << ")" << endl;
    double uk, vk, vr, vi, wk, f, deltaU, deltaV, ik, jk;
    while (!reader.is_done()) {
      vc = reader.next();
      vector<double> vis;  // string vector
      cout << "Task(" << id << ")" << endl;
      for (int i = 0; i < vc.size(); i++) {
        vis = str_to_vec(vc[i]);  // double vector
        uk = vis[0];
        vk = vis[1];
        vr = vis[3];
        vi = vis[4];
        wk = vis[5];
        f = vis[6];

        uk = uk * (f / speed_of_light);  // to wave longitude
        vk = vk * (f / speed_of_light);  // to wave longitude

        deltaU = 1 / (N * deg_to_rad(deltaX));  // to radians

        deltaV = deltaU;  // asuming deltav is equals to deltau
        ik = round((uk / deltaU) + (N / 2));
        jk = round((vk / deltaV) + (N / 2));
        cout << "uk: " << uk << " vk: " << vk << " vr: " << vr << " vi: " << vi
             << " wk: " << wk << " | ik(" << ik << ") jk(" << jk << ")" << endl;
        // fr[ik * N + jk] = fr[ik * N + jk] + (wk * vr);
        // fi[ik * N + jk] = fi[ik * N + jk] + (wk * vi);
        // wt[ik * N + jk] = wt[ik * N + jk] + wk;
      }
      reader.vtr.clear();
    }
    cout << "Fin Tarea(" << id << ")" << endl;
  }
};
void uMain::main() {
  string input_file_name;
  string output_file_name;
  double deltaX = 0.0;
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

  vector<double> fr;
  vector<double> fi;
  vector<double> wt;

  FileReader reader(input_file_name, c);  // Comonitor object creation
  MyTask* tasks[t];                       // Array of tasks

  for (int i = 0; i < t; i++) {
    tasks[i] = new MyTask(i, N, deltaX, reader);  // Allocation
  }

  for (int i = 0; i < t; i++) {
    delete tasks[i];
  }

  // fr = tasks[0]->fr;
  // fi = tasks[0]->fi;
  // wt = tasks[0]->wt;

  // for (int i = 0; i < N * N; i++) {
  //   if (wt[i] != 0) {
  //     fr[i] = fr[i] / wt[i];
  //     fi[i] = fi[i] / wt[i];
  //   }
  // }
  for (int i = 0; i < t; i++) {
    cout << tasks[i]->a << endl;
  }
  string r_file = output_file_name + "r.raw";
  string i_file = output_file_name + "i.raw";
}