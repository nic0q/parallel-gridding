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
const float SPEED_OF_LIGHT = 299792458;  // m/s

_Cormonitor FileReader {
 public:
  FileReader(string & filename, int chunk) : chunk(chunk) {
    file.open(filename.c_str());
  }
  ~FileReader() { file.close(); }

  vector<string> next() {
    resume();
    return vtr;
  }

  bool is_done() { return file.eof(); }

  void write_file(float* vc, int size, string& file) {
    FILE* outputFile = fopen(file.c_str(), "wb");
    if (!outputFile) {
      cerr << "Error al abrir el archivo: " << file << endl;
    }
    if (fwrite(vc, sizeof(float), size, outputFile) == size) {
      cout << "All elements were written successfully" << endl;
    } else {
      cout << "There was an error while writing the elements" << endl;
    }
    fclose(outputFile);
  }

 private:
  ifstream file;
  int chunk = 0;
  int n_line = 0;
  vector<string> vtr;

  void main() {
    string line;
    for (;;) {
      for (int i = 0; i < chunk; i++) {  // Reading X lines (chunk)
        if (!getline(file, line)) {      // Reached end of file
          break;
        }
        if (n_line % 500000 == 0) {  // cout each 500000 lines readed
          cout << "reading line: " << n_line << endl;
        }
        vtr.push_back(line);
        n_line += 1;
      }
      suspend();
      vtr.clear();
    }
  }
};
_Mutex class Mutex {
 public:
  Mutex(int size)
      : fr(new float[size]{}),
        fi(new float[size]{}),
        wt(new float[size]{}) {}  // Initialize matrix with 0's
  ~Mutex() {
    delete[] fr;
    delete[] fi;
    delete[] wt;
  }
  float* fr;
  float* fi;
  float* wt;

  float* get_fr() { return fr; }
  float* get_fi() { return fi; }
  float* get_wt() { return wt; }

  void set_fr(float v, int pos) { fr[pos] += v; }
  void set_fi(float v, int pos) { fi[pos] += v; }
  void set_wt(float v, int pos) { wt[pos] += v; }
};

_Task MyTask {
 public:
  MyTask(int id, int N, float deltaX, FileReader& reader, Mutex& mutex)
      : id(id), N(N), deltaX(deltaX), reader(reader), mutex(mutex) {}
  ~MyTask() { vc.clear(); }

  int id;
  int N;
  float deltaX;
  FileReader& reader;
  vector<string> vc;
  Mutex& mutex;

 private:
  float arcsec_to_rad(float deg) {  // arcseconds to radians
    return deg * M_PI / (180 * 3600);
  }

  vector<float> str_to_vec(string str) {  // string vector to float
    stringstream ss(str);
    vector<float> vis;
    while (ss.good()) {
      string substr;
      getline(ss, substr, ',');
      vis.push_back(stof(substr));
    }
    return vis;
  }

  void main() {
    cout << "Starting Task(" << id << ")" << endl;
    float uk, vk, vr, vi, wk, fq, deltaU, deltaV, ik, jk;

    vector<float> vis;                         // string vector
    deltaU = 1 / (N * arcsec_to_rad(deltaX));  // to radians
    deltaV = deltaU;  // asuming deltav is equals to deltau

    while (!reader.is_done()) {
      vc = reader.next();
      for (int i = 0; i < vc.size(); i++) {
        vis = str_to_vec(vc[i]);  // float vector
        uk = vis[0];
        vk = vis[1];
        vr = vis[3];
        vi = vis[4];
        wk = vis[5];
        fq = vis[6];

        uk = uk * (fq / SPEED_OF_LIGHT);  // to wave longitude
        vk = vk * (fq / SPEED_OF_LIGHT);  // to wave longitude

        ik = round(uk / deltaU) + (N / 2);  // i,j grid coordinate
        jk = round(vk / deltaV) + (N / 2);

        mutex.set_fr(wk * vr, ik * N + jk);  // acumulate with em
        mutex.set_fi(wk * vi, ik * N + jk);
        mutex.set_wt(wk, ik * N + jk);
      }
    }
    cout << "Ending Task(" << id << ")" << endl;
  }
};

void uMain::main() {
  string input_file_name, output_file_name, r_file_name, i_file_name;
  int option, N = 0, c = 0, t = 0, dim = 0;
  double tp = 0.0, time = 0.0;
  float deltaX = 0.0;
  unsigned t0, t1;
  float *fr, *fi, *wt;

  while ((option = getopt(argc, argv, "i:o:d:N:c:t:")) != -1) {
    switch (option) {
      case 'i':
        input_file_name = optarg;
        break;
      case 'o':
        output_file_name = optarg;
        break;
      case 'd':
        deltaX = stof(optarg);
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
  cout << "--------- Shared Matrix Method ---------" << endl;
  cout << "Input File: " << input_file_name << endl;
  cout << "Output File: " << output_file_name << endl;
  cout << "DeltaX: " << deltaX << endl;
  cout << "Image Size: " << N << endl;
  cout << "Chunk Size: " << c << endl;
  cout << "Number of Tasks: " << t << endl;

  dim = N * N;                            // Image dimension
  FileReader reader(input_file_name, c);  // Comonitor object creation
  Mutex matrices(dim);                    // Mutex
  MyTask* tasks[t];                       // Array of t tasks

  t0 = clock();
  for (int i = 0; i < t; i++) {  // Task creation
    tasks[i] = new MyTask(i, N, deltaX, reader, matrices);
  }
  for (int i = 0; i < t; i++) {  // Task finalization
    delete tasks[i];
  }
  fr = matrices.get_fr();
  fi = matrices.get_fi();
  wt = matrices.get_wt();

  for (int i = 0; i < dim; i++) {  // Weight normalization
    if (wt[i] != 0) {
      fr[i] = fr[i] / wt[i];
      fi[i] = fi[i] / wt[i];
    }
  }
  t1 = clock();
  time = double(t1 - t0) / CLOCKS_PER_SEC;

  r_file_name = output_file_name + "r.raw";  // Add file extension
  i_file_name = output_file_name + "i.raw";

  reader.write_file(fr, dim, r_file_name);  // Write gridding files
  reader.write_file(fi, dim, i_file_name);

  cout << "Shared Matrix Method time: " << time << "[s]" << endl;
}