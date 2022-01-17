#pragma once
#include <condition_variable>
#include <filesystem>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>

#include "sd/framework/logging/channel.h"
#include "sd/framework/logging/log_level_t.h"

namespace sd {
namespace framework {
namespace logging {

    class Logger {
    public:
        typedef std::basic_ostream<char, std::char_traits<char>>& (*manip_func_t)(std::basic_ostream<char, std::char_traits<char>>&);

        Logger();
        ~Logger();

        void AddChannel(const std::ostream& stream, log_level_t level = log_level_t::info);
        void AddChannel(const std::filesystem::path& path, log_level_t level = log_level_t::info);

        inline log_level_t LogLevel() const noexcept;
        inline void LogLevel(log_level_t level);

        template<class T>
        Logger& operator<<(const T& message);

        inline
        Logger& operator<<(manip_func_t manip_func);

    private:
        class LoggingAction {
        public:
            inline LoggingAction(std::ostream& stream);

            virtual void Log() = 0;
        protected:
            inline std::shared_ptr<std::ostream> Stream();
        private:
            std::shared_ptr<std::ostream> m_Stream;
        };

        template<class T>
        class LogMessageAction :
            public LoggingAction {
        public:
            LogMessageAction(std::ostream& stream, const T& message);
            inline virtual void Log() override;
        private:
            const T& m_Message;
        };

        class LogManipAction :
            public LoggingAction {
        public:
            inline LogManipAction(std::ostream& stream, manip_func_t manip_func);
            inline virtual void Log() override;
        private:
            manip_func_t m_ManipFunc;
        };

        Logger(const Logger&) = delete;
        Logger(Logger&&) = delete;

        template<class T>
        void Log(const T& message);
        
        inline
        void Log(manip_func_t manip_func);

        inline
        void Log(log_level_t level);

        void LoggingLoop();

        std::shared_ptr<LoggingAction> GetNextLoggingAction();

        std::vector<Channel> m_Channels;
        std::mutex m_LoggerMutex;
        std::queue<std::shared_ptr<LoggingAction>> m_MessageQueue;
        std::condition_variable m_MessageReady;
        bool m_Stop;
        std::future<void> m_LoggingLoop;
        log_level_t m_LogLevel;
    };

    Logger::LoggingAction::LoggingAction(std::ostream& stream) :
        m_Stream(std::make_shared<std::ostream>(stream.rdbuf())) {}

    std::shared_ptr<std::ostream> Logger::LoggingAction::Stream() {
        return m_Stream;
    }

    template<class T>
    Logger::LogMessageAction<T>::LogMessageAction(std::ostream& stream, const T& message) :
        Logger::LoggingAction(stream),
        m_Message(message) {}

    template<class T>
    void Logger::LogMessageAction<T>::Log() {
        auto s = Stream();
        if (s) {
            (*s) << m_Message;
        }
    }

    Logger::LogManipAction::LogManipAction(std::ostream& stream, manip_func_t manip_func) :
        Logger::LoggingAction(stream),
        m_ManipFunc(manip_func) {}

    void Logger::LogManipAction::Log() {
        auto s = Stream();
        if (s) {
            m_ManipFunc(*s);
        }
    }

    template<class T>
    void Logger::Log(const T& message) {
        std::unique_lock<std::mutex> locker(m_LoggerMutex);
        for (auto channel : m_Channels) {
            if (!channel.stream || channel.log_level > m_LogLevel) {
                continue;
            }
            auto action = std::shared_ptr<Logger::LoggingAction>(new Logger::LogMessageAction(*(channel.stream), message));
            m_MessageQueue.push(action);
        }

        m_MessageReady.notify_all();
    }

    void Logger::Log(manip_func_t manip_func) {
        std::unique_lock<std::mutex> locker(m_LoggerMutex);
        for (auto channel : m_Channels) {
            if (!channel.stream || channel.log_level > m_LogLevel) {
                continue;
            }
            auto action = std::shared_ptr<Logger::LoggingAction>(new Logger::LogManipAction(*(channel.stream), manip_func));
            m_MessageQueue.push(action);
        }

        m_MessageReady.notify_all();
    }

    void Logger::Log(log_level_t level) {
        LogLevel(level);
    }

    log_level_t Logger::LogLevel() const noexcept {
        return m_LogLevel;
    }

    void Logger::LogLevel(log_level_t level) {
        m_LogLevel = level;
    }

    template<class T>
    Logger& Logger::operator<<(const T& message) {
        Log(message);
        return *this;
    }

    Logger& Logger::operator<<(manip_func_t manip_func) {
        Log(manip_func);
        return *this;
    }

}}}