/*
Autor: Florian Schwarzl
Datum: 2021-01-03

Beschreibung:
Pipe-Klasse zur Kommunikation zwischen Threads
*/

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

template <typename T>
class Pipe {
  private:
    std::queue<T> backend;
    std::mutex mtx;
    std::condition_variable not_empty;

    // Id des Workers, der der Empfaenger dieser Pipe ist
    int recipient;
  
  public:
    Pipe(int recipient_) {
        recipient = recipient_;
    }
    void push_value(T value) {
        std::lock_guard lg{mtx};
        backend.push(value);
        not_empty.notify_one();
    }
    
    T get_value() {
        std::unique_lock ul{mtx};
        not_empty.wait(ul, [this] { return backend.size(); });
        T value = backend.front();
        backend.pop();
        return value;
    }

    int get_recipient() {
        return recipient;
    }
};