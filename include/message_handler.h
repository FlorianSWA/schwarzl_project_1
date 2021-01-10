

#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "pipe.h"
#include "message.h"
#include "spdlog/sinks/basic_file_sink.h"

class MessageHandler {
  private:
    // Id des Workers, dem der Handler gehört
    int WorkerId;

    // Alle Nachrichten an den Worker werden in seine Inbox geschrieben
    Pipe<Message> inbox;

    // Enthaelt Zeiger zu den Inboxen der anderen Worker
    std::vector<Pipe<Message>*> outboxes;

    // Befindet sich der Worker im krit. Abschnitt
    bool* in_crit_section;

    // Will der Worker in den krit. Abschnitt eintreten
    bool* wants_to_enter;

    // Sind bereits alle Responses eingetroffen
    bool* got_all_ok;

    // Timestamp des aktuellen Requests
    std::chrono::system_clock::time_point enter_timestamp;

    // MesssageQueue
    std::queue<Message> mq;

    // Kann der Worker in den krit. Abschnitt eintreten
    std::condition_variable* can_enter;

    // File-Logger
    std::shared_ptr<spdlog::logger> file_logger;
    
    // Sendet eine OK-Response an den Worker mit der übergebenen Id
    void send_ok(int Id_);

  public:
    MessageHandler(int Id_, bool* in_crit_section_, bool* wants_to_enter_, bool* got_all_ok_, std::condition_variable* can_enter_, std::shared_ptr<spdlog::logger> logger);
    
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