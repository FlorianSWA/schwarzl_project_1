/*
Autor: Florian Schwarzl
Datum: 2021-01-03

Beschreibung:
Worker-Klasse, in der der Algorithmus implementiert wird.
*/

#pragma once

#include <vector>
#include "message_handler.h"
#include "pipe.h"
#include "message.h"
#include "spdlog/sinks/basic_file_sink.h"


class Worker {
  private:
    // Id des Workers
    int Id;

    // Befindet sich der Worker im krit. Abschnitt
    bool in_crit_section{false};

    // Will der Worker in den krit. Abschnitt eintreten
    bool wants_to_enter{false};

    // Sind bereits alle Responses eingetroffen
    bool got_all_ok{false};

    // Der MessageHandler dieses Workers
    MessageHandler mh;

    // Mutex für die Condition Variable
    std::mutex mtx;

    // Kann der Worker in den krit. Abschnitt eintreten
    std::condition_variable can_enter;

    // File-logger
    std::shared_ptr<spdlog::logger> file_logger;

  public:
    Worker(int Id_, std::shared_ptr<spdlog::logger> logger) : mh(Id_, &in_crit_section, &wants_to_enter, &got_all_ok, &can_enter, logger) {
        Id = Id_;
        file_logger = logger;
    };

    // Inbox eines anderen Workers zum Vektor hinzufügen
    void add_outbox(Pipe<Message>* sender_);

    // Liefert die Inbox dieses Workers
    Pipe<Message>* get_inbox();

    void operator()();
};