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
    REQ,
    OK,
    NONE
};

class Message {
  private:
    int sender;
    int recipient;
    MessageType type{MessageType::NONE};
    std::chrono::system_clock::time_point value;
  public:
    Message();
    Message(int sender_, int recipient_, MessageType mt);
    Message(int sender_, int recipient_, MessageType mt, std::chrono::system_clock::time_point value_);
    int get_sender();
    int get_recipient();
    MessageType get_message_type();
    std::string toString();
};
