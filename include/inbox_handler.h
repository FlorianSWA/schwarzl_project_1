

#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "pipe.h"
#include "message.h"

class InboxHandler {
  private:
    int WorkerId;
    Pipe<Message>* inbox;
    std::vector<Pipe<Message>*> outboxes;
    bool* in_crit_section;
    bool* wants_to_enter;
    std::chrono::system_clock::time_point enter_timestamp;
    std::queue<Message> mq;
    std::condition_variable* can_enter;
    
    void send_ok(int Id_);

  public:
    InboxHandler(int Id_, bool* in_crit_section_, bool* wants_to_enter_, std::condition_variable* con_var, Pipe<Message>* inbox_, std::vector<Pipe<Message>*> outboxes_);
    void operator()();
    
    // Outbox eines anderen Workers zum Vektor hinzufuegen
    void add_outbox(Pipe<Message>* sender_);

    // Liefert die Inbox dieses Workers
    Pipe<Message>* get_inbox();
    void set_enter_timestamp(std::chrono::system_clock::time_point tp);
    void done();

};