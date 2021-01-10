/*
Autor: Florian Schwarzl
Datum: 2021-01-03
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include "worker.h"
#include "pipe.h"
#include "message.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/bundled/color.h"
#include "spdlog/fmt/bundled/chrono.h"


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
        fmt::print("Worker {} no longer wants to enter critical section\n", Id);
        file_logger->info("Worker {} no longer wants to enter critical section", Id);
        this_thread::sleep_for(chrono::milliseconds(int (dis(gen) * 1000)));
        fmt::print("Worker {} wants to enter critical section.\n", Id);
        file_logger->info("Worker {} wants to enter critical section.");
        wants_to_enter = true;

        chrono::system_clock::time_point timestamp{chrono::system_clock::now()};
        mh.set_enter_timestamp(timestamp);
        mh.send_to_all(MessageType::REQ, timestamp);
        fmt::print(fg(fmt::color::blue), "Worker {} sent a Request with Timestamp {:%H:%M:%S} to all other Workers.\n", Id, timestamp);
        file_logger->info("Worker {} sent Request  to all other Workers",Id);

        unique_lock ul{mtx};
        can_enter.wait(ul, [this] { return got_all_ok; });
        in_crit_section = true;
        got_all_ok = false;
        fmt::print(fg(fmt::color::gold), "Worker {} entered critical section.\n", Id);
        file_logger->info("Worker {} entered critical section.", Id);
        this_thread::sleep_for(4s);
        fmt::print(fg(fmt::color::gold), "Worker {} left critical section.\n", Id);
        file_logger->info("Worker {} left critical section.", Id);
        in_crit_section = false;
        mh.done();
    }
    mh_thread.join();
}