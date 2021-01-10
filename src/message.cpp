/*
Autor: Florian Schwarzl
Datum: 2021-01-03

Beschreibung:
Message-Klasse, welche die Requests und Responses repraesentiert
*/

#include <string>
#include "message.h"
#include "magic_enum.hpp"


using namespace std;

Message::Message(int sender_, int recipient_, MessageType mt) {
    sender = sender_;
    recipient = recipient_;
    type = mt;   
}

Message::Message(int sender_, int recipient_, MessageType mt, chrono::system_clock::time_point value_) {
    sender = sender_;
    recipient = recipient_;
    type = mt;
    value = value_;
}

int Message::get_sender() {
    return sender;
}

int Message::get_recipient() {
    return recipient;
}

chrono::system_clock::time_point Message::get_value() {
    return value;
}

MessageType Message::get_message_type() {
    return type;
}

string Message::toString() {
    string enum_string{magic_enum::enum_name(type)};
    if (type == MessageType::REQ) {
        return "Message(sender=" + to_string(sender) + ", recipient=" + to_string(recipient) + "type=" + enum_string + ", value=" + to_string(chrono::system_clock::to_time_t(value)) +")";
    } else {
        return "Message(sender=" + to_string(sender) + ", recipient=" + to_string(recipient) + "type=" + enum_string + ")";
    }
    
}
