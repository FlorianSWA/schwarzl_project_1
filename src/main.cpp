/*
Autor: Florian Schwarzl
Kat. Nr: 21
*/

#include <iostream>
#include <thread>
#include "CLI11.hpp"
#include "worker.h"
#include "utils.h"

using namespace std;

int main(int argc, char* argv[]) {
    CLI::App app("Simuliert den verteilten Algorithmus zur Synchronisation");

    unsigned int worker_cnt{3};

    app.add_option("--workers, -w", worker_cnt, "Anzahl der Worker der Simulation.");

    CLI11_PARSE(app, argc, argv);
    
    vector<thread> thread_vector;
    vector<Worker*> worker_vector;
    
    for (int i{0}; i < worker_cnt; i++) {
        worker_vector.push_back(new Worker(i, worker_cnt));
    }
    
    for (int i{0}; i < worker_cnt; i++) {
        for (int j{0}; j < worker_cnt; j++) {
            if (i != j) {
                worker_vector[i]->add_outbox(worker_vector[j]->get_inbox());
            }
        }
    }

    for (int i{0}; i < worker_cnt; i++) {
        thread_vector.push_back(thread(ref(*worker_vector[i])));
    }

    for (int i{0}; i < worker_cnt; i++) {
        thread_vector[i].join();
    }
    for (int i{0}; i < worker_cnt; i++) {
        delete worker_vector[i];
    }
}
