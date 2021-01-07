
#include <vector>
#include "utils.h"
#include "message_handler.h"

using namespace std;

MessageHandler::MessageHandler(int Id_, bool* in_crit_section_, bool* wants_to_enter_, bool* got_all_ok_, condition_variable* can_enter_) : inbox(Id_) {
    WorkerId = Id_;
    can_enter = can_enter_;
    in_crit_section = in_crit_section_;
    wants_to_enter = wants_to_enter_;
    got_all_ok = got_all_ok_;
}

void MessageHandler::operator()() {
    unsigned int ok_cnt{0};
    while (true) {
        if (ok_cnt == outboxes.size()) {
            /*
            Condition Variable durch Latch ersetzten, um Lost wakeup zu verhindern.
            ODER
            Weitere Bool Variable, die im wait abgefragt wird.
            */
            can_enter->notify_one();
            *got_all_ok = true;
            ok_cnt = 0;
        }
        Message m{inbox.get_value()};
        if (m.get_sender() != WorkerId) {
            if (m.get_message_type() == MessageType::REQ) {
                if (!*wants_to_enter && !*in_crit_section) {
                    send_ok(m.get_sender());
                } else if (*wants_to_enter && !*in_crit_section) {
                    if (m.get_value() < enter_timestamp) {
                        send_ok(m.get_sender());
                    } else {
                        mq.push(m);
                    }
                } else if (*in_crit_section) {
                    mq.push(m);
                }
            } else if (m.get_message_type() == MessageType::OK){
                ok_cnt++;
            }
        }
    }
}

void MessageHandler::send_ok(int Id_) {
    for (size_t i{0}; i < outboxes.size(); i++) {
        if (outboxes[i]->get_owner() == Id_) {
            outboxes[i]->push_value(Message(WorkerId, outboxes[i]->get_owner(), MessageType::OK));
        }
    }
}

void MessageHandler::done() {
    while (mq.size()) {
        send_ok(mq.front().get_sender());
        mq.pop();
    }
    //println("Exec done");
}

void MessageHandler::set_enter_timestamp(std::chrono::system_clock::time_point tp) {
    enter_timestamp = tp;
}

void MessageHandler::add_outbox(Pipe<Message>* sender_) {
    outboxes.push_back(sender_);
}

Pipe<Message>* MessageHandler::get_inbox() {
    return &inbox;
}

void MessageHandler::send_to_all(MessageType type_, chrono::system_clock::time_point value_) {
    
    for (size_t i{0}; i < outboxes.size(); i++) {
        outboxes[i]->push_value(Message(WorkerId, outboxes[i]->get_owner(), type_, value_));
    }
    inbox.push_value(Message(WorkerId, WorkerId, type_, value_));
}