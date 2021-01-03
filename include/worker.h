/*
Autor: Florian Schwarzl
Datum: 2021-01-03

Beschreibung:
Worker-Klasse, in der der Algorithmus implementiert wird.
*/

#pragma once

#include <vector>
#include <queue>

class Worker {
  private:
    int Id;
    std::vector<int> worker_list;
    std::queue<int> worker_queue;
  public:
    Worker(int Id_);

    void operator()();
};