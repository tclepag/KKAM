#pragma once

#include <functional>
#include <vector>
#include <algorithm>

namespace utils {

    template<typename... Args>
    class Event {
    public:
        using Callback = std::function<void(Args...)>;

        void subscribe(const Callback& callback) {
            m_callbacks.push_back(callback);
        }

        void unsubscribe(const Callback& callback) {
            m_callbacks.erase(std::remove(m_callbacks.begin(), m_callbacks.end(), callback), m_callbacks.end());
        }

        void notify(Args... args) {
            for (const auto& callback : m_callbacks) {
                callback(args...);
            }
        }

    private:
        std::vector<Callback> m_callbacks;
    };

}
