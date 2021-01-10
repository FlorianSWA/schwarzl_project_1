/*
Autor: Florian Schwarzl
Datum: 2021-01-03

Beschreibung:
Message-Klasse, welche die Requests und Responses repraesentiert
*/

#pragma once

#include <chrono>
#include <string>

enum class MessageType {
    REQ = 1,
    OK = 2,
    NONE = 0
};

class Message {
  private:
    int sender;
    int recipient;
    MessageType type{MessageType::NONE};
    std::chrono::system_clock::time_point value;
  public:
    Message(int sender_, int recipient_, MessageType mt);
    Message(int sender_, int recipient_, MessageType mt, std::chrono::system_clock::time_point value_);
    int get_sender();
    int get_recipient();
    std::chrono::system_clock::time_point get_value();
    MessageType get_message_type();
    std::string toString();
};
