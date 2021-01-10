/*
Autor: Florian Schwarzl
Kat. Nr: 21
*/

#include <iostream>
#include <thread>
#include "CLI11.hpp"
#include "worker.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/bundled/color.h"


using namespace std;


int main(int argc, char* argv[]) {
    CLI::App app("Simulate the distributed algorithm for synchronization");

    unsigned int worker_cnt{3};
    bool use_logging{false};
    bool log_level_debug{false};

    app.add_option("workers", worker_cnt, "Number of Nodes (default = 3).")->check(CLI::NonNegativeNumber);
    auto log_flag{app.add_flag("-l, --log", use_logging, "Write log file dist_sync_log.log")};
    app.add_flag("-d, --debug", log_level_debug, "Set log level to debug.")->needs(log_flag);

    CLI11_PARSE(app, argc, argv);

    shared_ptr<spdlog::logger> file_logger{spdlog::basic_logger_mt("basic_logger", "./dist_sync_log.log")};

    if (use_logging) {
        if (log_level_debug) {
            file_logger->set_level(spdlog::level::debug);
            file_logger->flush_on(spdlog::level::debug);
        } else {
            file_logger->set_level(spdlog::level::info);
            file_logger->flush_on(spdlog::level::info);
        }
    } else {
        file_logger->set_level(spdlog::level::off);
        file_logger->flush_on(spdlog::level::off);
    }
    
    vector<thread> thread_vector;
    vector<Worker*> worker_vector;

    file_logger->info("Started simulation with {} Worker Threads", worker_cnt);
    file_logger->info("Options: --log {}, --debug: {}", use_logging, log_level_debug);
    fmt::print(fg(fmt::color::dark_green) | fmt::emphasis::bold
              , "Started simulation with {} Worker Threads.\nOptions: --log: {}, --debug: {}\n\n"
              , worker_cnt, use_logging, log_level_debug);

    for (unsigned int i{0}; i < worker_cnt; i++) {
        worker_vector.push_back(new Worker(i, file_logger));
        file_logger->info("Created Worker {}", i);
    }
    
    for (unsigned int i{0}; i < worker_cnt; i++) {
        for (unsigned int j{0}; j < worker_cnt; j++) {
            if (i != j) {
                worker_vector[i]->add_outbox(worker_vector[j]->get_inbox());
                file_logger->debug("Added inbox of Worker {0} to outbox of Worker {1}", j, i);
            }
        }
    }

    for (unsigned int i{0}; i < worker_cnt; i++) {
        thread_vector.push_back(thread(ref(*worker_vector[i])));
        file_logger->info("Started Worker {} as Thread", i);
    }

    for (unsigned int i{0}; i < worker_cnt; i++) {
        thread_vector[i].join();
    }
    for (unsigned int i{0}; i < worker_cnt; i++) {
        delete worker_vector[i];
    }
}
