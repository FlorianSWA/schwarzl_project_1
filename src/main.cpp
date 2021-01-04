/*
Autor: Florian Schwarzl
Kat. Nr: 21
*/

#include <iostream>
#include <thread>
#include "worker.h"

using namespace std;

//int main(int argc, char* argv[]) {
int main() {
    const int max_threads{3};
    
    thread thread_array[max_threads];
    vector<Worker*> worker_vector;
    
    for (int i{0}; i < max_threads; i++) {
        worker_vector.push_back(new Worker(i, max_threads));
    }
    
    for (int i{0}; i < max_threads; i++) {
        for (int j{0}; j < max_threads; j++) {
            if (i != j) {
                worker_vector[i]->add_outbox(worker_vector[j]->get_inbox());
            }
        }
    }

    for (int i{0}; i < max_threads; i++) {
        thread_array[i] = thread(ref(*worker_vector[i]));
    }

    for (int i{0}; i < max_threads; i++) {
        thread_array[i].join();
    }
    
    for (int i{0}; i < max_threads; i++) {
        delete worker_vector[i];
    }
}
