/*
Autor: Florian Schwarzl
Datum: 2021-01-03

Beschreibung:
Worker-Klasse, in der der Algorithmus implementiert wird.
*/

#pragma once

#include <vector>
#include "pipe.h"
#include "message.h"


class Worker {
  private:
    int Id;
    int total_workers;

    //Alle Nachrichten an den Worker werden in seine Inbox geschrieben
    Pipe<Message> inbox;

    // Enthaelt Zeiger zu den Inboxen der anderen Worker
    std::vector<Pipe<Message>*> outboxes;

    // Sendet eine Nachricht an alle Worker, die in Outboxes angelegt sind
    void send_to_all(MessageType type_, std::chrono::system_clock::time_point value_);
  public:
    Worker(int Id_, int total_workers_) : inbox(Id_) {
        Id = Id_;
        total_workers = total_workers_;
        outboxes.reserve(total_workers_ - 1);
    };
    // Outbox eines anderen Workers zum Vektor hinzufuegen
    void add_outbox(Pipe<Message>* sender_);

    // Liefert die Inbox dieses Workers
    Pipe<Message>* get_inbox();
    void operator()();
};