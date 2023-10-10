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

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

_Cormonitor FileReader {
 public:
  std::vector<string> vtr;
  FileReader(string & filename, int chunkSize) : chunk(chunkSize) {
    file.open(filename.c_str());
  }
  ~FileReader() { file.close(); }
  std::vector<string> next() {
    resume();
    return vtr;
  }
  bool finish() { return fin; }
  void clean_vtr() { vtr.clear(); }

 private:
  bool fin = false;
  ifstream file;
  int chunk;

  void main() {
    cout << "INICIO CORRUTINA" << endl;
    string line;
    while (true) {  // leer X chunk lines
      suspend();
      for (int i = 0; i < chunk; i++) {
        if (!getline(file, line)) {
          fin = true;
          break;
        } else {
          vtr.push_back(line);
          cout << "FileReader(" << i << ") State " << vtr.size() << endl;
        }
      }
    }
  }
};

_Task MyTask {
 public:
  MyTask(int taskId, FileReader& reader) : id(taskId), reader(reader) {}
  bool end = false;
  int id;
  FileReader& reader;
  std::vector<string> vc;

 private:
  void main() {
    cout << "INICIO TAREA " << id << endl;
    while (!reader.finish()) {
      vc = reader.next();
      cout << "TAMAÑO VECTOR " << (vc.size() == 0 ? "VACIO" : vc[0]) << endl;
      // cout << "TAREA[" << vc.size() << " O.O" << id << "] ESTADO ARCHIVO["
      //      << (reader.finish() == 1 ? "FINALIZADO]" : "EN CURSO]") << endl;
      reader.vtr.clear();
    }
    cout << "Fin Tarea(" << id << ")" << endl;
  }
};
int main(int argc, char* argv[]) {
  // Variables para almacenar los argumentos
  string inputFileName;
  string outputFileName;
  double deltau = 0.0;
  int N = 0;
  int chunkSize = 0;
  int t = 0;

  // Procesar los argumentos de línea de comandos utilizando getopt
  int option;
  while ((option = getopt(argc, argv, "i:o:d:N:c:t:")) != -1) {
    switch (option) {
      case 'i':
        inputFileName = optarg;
        break;
      case 'o':
        outputFileName = optarg;
        break;
      case 'd':
        deltau = stod(optarg);
        break;
      case 'N':
        N = stoi(optarg);
        break;
      case 'c':
        chunkSize = stoi(optarg);
        break;
      case 't':
        t = stoi(optarg);
        break;
      default:
        cerr << "Uso: " << argv[0]
             << " -i input -o output -d deltau -N tamaño -c chunk -t tareas"
             << endl;
        return 1;
    }
  }
  cout << "Input File: " << inputFileName << endl;
  cout << "Output File: " << outputFileName << endl;
  cout << "Deltau: " << deltau << endl;
  cout << "Image Size: " << N << endl;
  cout << "Chunk Size: " << chunkSize << endl;
  cout << "Number of Tasks: " << t << endl;

  float *fr, *fi, *wt;
  fr = new float[N * N];
  fi = new float[N * N];
  wt = new float[N * N];

  FileReader reader(inputFileName, chunkSize);

  MyTask** tasks = new MyTask*[t];  // Arreglo de punteros a tareas
  for (int i = 0; i < t; i++) {
    tasks[i] = new MyTask(i, reader);  // Crear una tarea y almacenarla
  }
  // Esperar a que todas las tareas terminen
  for (int i = 0; i < t; i++) {
    delete tasks[i];  // Liberar la memoria de las tareas
  }

  delete[] tasks;  // Liberar el arreglo de tareas
  delete[] fr;
  delete[] fi;
  delete[] wt;
  return 0;
}
