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

_Task MyTask {
 public:
  MyTask(int id) : id(id) {}

 private:
  int id;

  void main() {
    for (int i = 0; i < 5; i++) {
      cout << "TAREA" << id << endl;
      yield();
    }
  }
};

void uMain::main() {
  MyTask *tasks[3];
  for (int i = 0; i < 3; i++) {  // Task creation
    tasks[i] = new MyTask(i);
  }
  for (int i = 0; i < 3; i++) {  // Task finalization
    delete tasks[i];
  }
}