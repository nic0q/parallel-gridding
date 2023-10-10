/* Grideo
Antenas miden señales de algun objeto muy lejado
a medida que se va moviendo va leyendo, entrega valores complejos van
produciendo datos en el plano [u,v] (de Fourier), muchas antenas que siguen el
mismo objeto me dan puntos distintos. Cada punto se llama Visibilidad (emiten
señales de radiofrecuencia). Hay que transformar a una imagen de intensidad
(señal -> imagen)[Síntesis de imágenes] Cada visibilidad tiene puntos reales e
imaginarios, por lo tanto si uno le aplica la inversa de Fourier debería obtener
la imagen, pero esto no es así dado que hay mucho espacio y 0's Ademas estamos
en el plano contínuo, no es una matriz, por lo que Gridding lo transforma a una
matriz
->Gridding<-
plano continuo -> discreto(matriz cuadrada) donde en cada punto hay que sumar
todos los que estan en una determinada vecindad 1- matriz parte real 2- matriz
parte imaginaria 3- matriz compleja (a partir de 1 y 2) -> inversa
fourier(imagen en intensidad) [abs (r), abs(i), abs()] .raw para ver la imagen
cada visibilidad tmb tiene un PESO el cual mide la incertidumbre, es decir
mientras MAS ALTO = MAS PRECISO multiplicar valor real * peso | valor imaginario
* peso (normalización con los pesos)
*/

#include <stdbool.h>
#include <uC++.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
const double speed_of_light = 2.998 * 1e+8;

_Cormonitor FileReader {
 public:
  FileReader(string & filename, int chunkSize) : chunk(chunkSize) {
    file.open(filename.c_str());
  }

  ~FileReader() { file.close(); }

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

  void main() {
    string line;
    while (true) {
      for (int i = 0; i < chunk; i++) {  // Reading X lines of chunk
        if (!getline(file, line)) {
          break;
        } else {
          vtr.push_back(line);
          cout << "read" << endl;
        }
      }
      suspend();
    }
  }
};

_Task MyTask {
 public:
  MyTask(int id, int N, double deltaX, FileReader& reader)
      : id(id), N(N), deltaX(deltaX), reader(reader) {}

  int id;
  int N;
  double deltaX;
  double** fr;
  double** fi;
  double** wt;
  FileReader& reader;
  vector<string> vc;

 private:
  double deg_to_rad(double deg) { return deg * (M_PI / 180); }

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
    while (!reader.is_done()) {
      vc = reader.next();
      vector<double> vis;  // string vector
      cout << "Task(" << id << ")" << endl;
      for (int i = 0; i < vc.size(); i++) {
        vis = str_to_vec(vc[i]);  // double vector
        double uk, vk, vr, vi, w, f, deltaU, deltaV, ik, jk;
        int spc;
        uk = vis[0];
        vk = vis[1];
        vr = vis[3];
        vi = vis[4];
        w = vis[5];
        f = vis[6];
        spc = vis[7];
        deltaU = 1 / (N * deg_to_rad(deltaX));  // to radians
        deltaU = deltaU * f / speed_of_light;   // to wave longitude
        deltaV = deltaU;  // asuming deltav is equals to deltau
        ik = round((uk / deltaU) + (N / 2));
        jk = round((vk / deltaV) + (N / 2));
        cout << "uk: " << uk << " vk: " << vk << " vr: " << vr << " vi: " << vi
             << " | ik(" << ik << ") jk(" << jk << ")" << endl;
      }
      reader.vtr.clear();
    }
    cout << "Fin Tarea(" << id << ")" << endl;
  }
};
int main(int argc, char* argv[]) {
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
        return 1;
    }
  }
  cout << "Input File: " << input_file_name << endl;
  cout << "Output File: " << output_file_name << endl;
  cout << "DeltaX: " << deltaX << endl;
  cout << "Image Size: " << N << endl;
  cout << "Chunk Size: " << c << endl;
  cout << "Number of Tasks: " << t << endl;

  float *fr, *fi, *wt;
  fr = new float[N * N];
  fi = new float[N * N];
  wt = new float[N * N];

  FileReader reader(input_file_name, c);  // Comonitor object creation
  MyTask** tasks = new MyTask*[t];        // Array of tasks

  for (int i = 0; i < t; i++) {
    tasks[i] = new MyTask(i, N, deltaX, reader);  // Allocation
  }

  for (int i = 0; i < t; i++) {
    delete tasks[i];
  }

  delete[] tasks;
  delete[] fr;
  delete[] fi;
  delete[] wt;
  return 0;
}
