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


class Worker {
  private:
    // Id des Workers
    int Id;
    bool in_crit_section{false};
    bool wants_to_enter{false};
    bool got_all_ok{false};
    MessageHandler mh;
    std::mutex mtx;
    std::condition_variable can_enter;

  public:
    Worker(int Id_) : mh(Id_, &in_crit_section, &wants_to_enter, &got_all_ok, &can_enter) {
        Id = Id_;
    };

    // Inbox eines anderen Workers zum Vektor hinzufügen
    void add_outbox(Pipe<Message>* sender_);

    // Liefert die Inbox dieses Workers
    Pipe<Message>* get_inbox();

    void operator()();
};