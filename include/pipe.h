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
  
    bool closed{false};
  public:
    Pipe(int recipient_) {
        recipient = recipient_;
    }
    Pipe& operator<<(T value) {
        if (!closed) {
            std::lock_guard lg{mtx};
            backend.push(value);
            not_empty.notify_one();
        }
        return *this;
    }
    
    Pipe& operator>>(T& value) {
        if (!closed) {
            std::unique_lock ul{mtx};
            not_empty.wait(ul, [this] { return backend.size(); });
            value = backend.front();
            backend.pop();
        }
        return *this;
    }

    void close() {
        closed = true;
    }
    
    explicit operator bool() {
        return !closed;
    }

    int get_recipient() {
        return recipient;
    }
};