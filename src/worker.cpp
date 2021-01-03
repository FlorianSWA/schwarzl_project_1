/*
Autor: Florian Schwarzl
Datum: 2021-01-03
*/

#include <iostream>
#include "worker.h"

using namespace std;

Worker::Worker(int Id_) {
    Id = Id_;
}

void Worker::operator()() {
    cout << "Worker: " << Id << endl;
}