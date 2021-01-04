
#include <vector>
#include "utils.h"
#include "inbox_handler.h"

using namespace std;

InboxHandler::InboxHandler(int Id_, bool* in_crit_section_, bool* wants_to_enter_, condition_variable* con_var, Pipe<Message>* inbox_, std::vector<Pipe<Message>*> outboxes_) {
    WorkerId = Id_;
    can_enter = con_var;
    inbox = inbox_;
    outboxes = outboxes_;
    in_crit_section = in_crit_section_;
    wants_to_enter = wants_to_enter_;
}

void InboxHandler::operator()() {
    unsigned int ok_cnt{0};
    while (true) {
        if (ok_cnt == outboxes.size()) {
            //println(WorkerId, "all checked");
            /*
            Condition Variable durch Latch ersetzten, um Lost wakeup zu verhindern.
            ODER
            Weitere Bool Variable, die im wait abgefragt wird.
            */
            can_enter->notify_one();
            ok_cnt = 0;
        }
        Message m{inbox->get_value()};
        //println("Worker", WorkerId, "recieved Message", m.toString());
        if (m.get_message_type() == MessageType::REQ) {
            //println(*wants_to_enter, *in_crit_section);
            if (!*wants_to_enter && !*in_crit_section) {
                send_ok(m.get_sender());
                //println("Sent Ok form", WorkerId, "to", m.get_sender());
            } else if (*wants_to_enter && !*in_crit_section) {
                if (m.get_value() < enter_timestamp) {
                    //println("Compare: Ok to sender");
                    send_ok(m.get_sender());
                } else {
                    mq.push(m);
                    //println("Req in queue");
                }
            } else if (*in_crit_section) {
                mq.push(m);
            }
        } else if (m.get_message_type() == MessageType::OK){
            //println("Worker", WorkerId, "recieved Ok");
            ok_cnt++;
        }
    }
}

void InboxHandler::send_ok(int Id_) {
    for (size_t i{0}; i < outboxes.size(); i++) {
        if (outboxes[i]->get_owner() == Id_) {
            outboxes[i]->push_value(Message(WorkerId, outboxes[i]->get_owner(), MessageType::OK));
        }
    }
}

void InboxHandler::done() {
    while (mq.size()) {
        send_ok(mq.front().get_sender());
        mq.pop();
    }
    //println("Exec done");
}

void InboxHandler::set_enter_timestamp(std::chrono::system_clock::time_point tp) {
    enter_timestamp = tp;
}