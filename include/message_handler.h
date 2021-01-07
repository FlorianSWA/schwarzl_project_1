

#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "pipe.h"
#include "message.h"

class MessageHandler {
  private:
    // Id des Workers, dem der Handler gehört
    int WorkerId;

    // Alle Nachrichten an den Worker werden in seine Inbox geschrieben
    Pipe<Message> inbox;

    // Enthaelt Zeiger zu den Inboxen der anderen Worker
    std::vector<Pipe<Message>*> outboxes;

    bool* in_crit_section;
    bool* wants_to_enter;
    bool* got_all_ok;
    std::chrono::system_clock::time_point enter_timestamp;
    std::queue<Message> mq;

    std::condition_variable* can_enter;
    
    void send_ok(int Id_);

  public:
    MessageHandler(int Id_, bool* in_crit_section_, bool* wants_to_enter_, bool* got_all_ok_, std::condition_variable* can_enter_);
    void operator()();
    
    // Outbox eines anderen Workers zum Vektor hinzufügen
    void add_outbox(Pipe<Message>* sender_);

    // Liefert die Inbox dieses Workers
    Pipe<Message>* get_inbox();

    // setzt den Timestamp für das Eintreten in den krit. Abschnitt
    void set_enter_timestamp(std::chrono::system_clock::time_point tp);

    // sendet eine Message mit den übergebenen Werten an alle Worker, die in Outboxes angelegt sind
    void send_to_all(MessageType type_, std::chrono::system_clock::time_point value_);

    // Schickt an alle während dem krit. Abschnitt erhaltenen REQ ein OK zurück
    void done();

};