/*
Autor: Florian Schwarzl
Datum: 2021-01-03

Beschreibung:
Worker-Klasse, in der der Algorithmus implementiert wird.
*/

#pragma once

#include <vector>
#include <queue>
#include "pipe.h"


class Worker {
  private:
    int Id;
    int total_workers;

    //Alle Nachrichten an den Worker werden in seine Inbox geschrieben
    Pipe<std::string> inbox;

    // Enthaelt Zeiger zu den Inboxen der anderen Worker
    std::vector<Pipe<std::string>*> outboxes;

    // Value wird an alle Worker, die in Outboxes angelegt sind, gesendet
    void send_to_all(std::string value);
  public:
    Worker(int Id_, int total_workers_) : inbox(Id_) {
        Id = Id_;
        total_workers = total_workers_;
    };
    // Outbox eines anderen Workers zum Vektor hinzufuegen
    void add_outbox(Pipe<std::string>* sender_);

    // Liefert die Inbox dieses Workers
    Pipe<std::string>* get_inbox();
    void operator()();
};