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
    mh.add_outbox(sender_);
}

Pipe<Message>* Worker::get_inbox() {
    return mh.get_inbox();
}


void Worker::operator()() {
    thread mh_thread(ref(mh));
    random_device rd{};
    mt19937 gen{rd()};
    uniform_real_distribution<double> dis{3, 5};
    while (true) {
        wants_to_enter = false;
        this_thread::sleep_for(chrono::milliseconds(int (dis(gen) * 1000)));
        wants_to_enter = true;

        chrono::system_clock::time_point timestamp{chrono::system_clock::now()};
        mh.set_enter_timestamp(timestamp);
        mh.send_to_all(MessageType::REQ, timestamp);

        unique_lock ul{mtx};
        can_enter.wait(ul, [this] { return got_all_ok; });
        in_crit_section = true;
        got_all_ok = false;
        println("Worker ", Id, " entered critical section.");
        this_thread::sleep_for(4s);
        println("Worker ", Id, " left critical section.");
        in_crit_section = false;
        mh.done();
    }
    mh_thread.join();
}