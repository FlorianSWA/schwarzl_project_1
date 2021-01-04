/*
Autor: Florian Schwarzl
Datum: 2021-01-03
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <random>
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
        outboxes[i]->push_value(Message(Id, outboxes[i]->get_owner(), type_, value_));
    }
    
}

void Worker::operator()() {
    InboxHandler* IbH{new InboxHandler(Id, &in_crit_section, &wants_to_enter, &can_enter, &inbox, outboxes)};
    thread IbH_thread(ref(*IbH));
    random_device rd{};
    mt19937 gen{rd()};
    uniform_int_distribution<unsigned int> dis{3, 5};
    while (true) {
        wants_to_enter = false;

        this_thread::sleep_for(chrono::seconds(dis(gen)));
        wants_to_enter = true;
        chrono::system_clock::time_point timestamp{chrono::system_clock::now()};
        IbH->set_enter_timestamp(timestamp);
        send_to_all(MessageType::REQ, timestamp);
        unique_lock ul{mtx};
        can_enter.wait(ul);
        in_crit_section = true;
        println("Worker ", Id, " entered critical section.");
        this_thread::sleep_for(4s);
        in_crit_section = false;
        println("Worker ", Id, " left critical section.");
        IbH->done();
    }
    IbH_thread.join();
    delete IbH;
}