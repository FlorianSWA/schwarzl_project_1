/*
Autor: Florian Schwarzl
Datum: 2021-01-03
*/

#include <iostream>
#include "worker.h"
#include "utils.h"


using namespace std;


void Worker::add_outbox(Pipe<string>* sender_) {
    outboxes.push_back(sender_);
}

Pipe<string>* Worker::get_inbox() {
    return &inbox;
}

void Worker::send_to_all(string value) {
    for (size_t i{0}; i < outboxes.size(); i++) {
        *outboxes[i] << value;
    }
}

void Worker::operator()() {
    println("Worker: ", Id);
    send_to_all("Hello from Worker " + to_string(Id));
    string value;
    while (inbox >> value) {
        println("Worker: ", Id, " -- ", value);
    }
}