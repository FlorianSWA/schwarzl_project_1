/*
Autor: Florian Schwarzl
Datum: 2021-01-03
*/

#include <iostream>
#include <chrono>
#include "worker.h"
#include "utils.h"
#include "pipe.h"
#include "message.h"


using namespace std;


void Worker::add_outbox(Pipe<Message>* sender_) {
    outboxes.push_back(sender_);
}

Pipe<Message>* Worker::get_inbox() {
    return &inbox;
}

void Worker::send_to_all(MessageType type_, chrono::system_clock::time_point value_) {
    
    for (size_t i{0}; i < outboxes.size(); i++) {
        outboxes[i]->push_value(Message(Id, outboxes[i]->get_recipient(), type_, value_));
    }
    
}

void Worker::operator()() {
    
    println("Worker: ", Id);
    send_to_all(MessageType::REQ, chrono::system_clock::now());
    while (true) {
        Message m{inbox.get_value()};
        println("Worker: ", to_string(Id), " -- recieved Message: ", m.toString());
    }
}