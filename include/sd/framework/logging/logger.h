#pragma once
#include <condition_variable>
#include <filesystem>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>

namespace sd {
namespace framework {
namespace logging {

    class Logger {
    public:
        typedef std::basic_ostream<char, std::char_traits<char>>& (*manip_func_t)(std::basic_ostream<char, std::char_traits<char>>&);

        Logger();
        Logger(const std::filesystem::path& file_path);
        ~Logger();

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
            inline std::ostream& Stream();
        private:
            std::ostream& m_Stream;
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

        void LoggingLoop();

        std::shared_ptr<LoggingAction> GetNextLoggingAction();

        std::ostream& m_OutputStream;
        std::mutex m_LoggerMutex;
        std::queue<std::shared_ptr<LoggingAction>> m_MessageQueue;
        std::condition_variable m_MessageReady;
        bool m_Stop;
        std::future<void> m_LoggingLoop;
        std::shared_ptr<std::ofstream> m_FileStream;
    };

    Logger::LoggingAction::LoggingAction(std::ostream& stream) :
        m_Stream(stream) {}

    std::ostream& Logger::LoggingAction::Stream() {
        return m_Stream;
    }

    template<class T>
    Logger::LogMessageAction<T>::LogMessageAction(std::ostream& stream, const T& message) :
        Logger::LoggingAction(stream),
        m_Message(message) {}

    template<class T>
    void Logger::LogMessageAction<T>::Log() {
        Stream() << m_Message;
    }

    Logger::LogManipAction::LogManipAction(std::ostream& stream, manip_func_t manip_func) :
        Logger::LoggingAction(stream),
        m_ManipFunc(manip_func) {}

    void Logger::LogManipAction::Log() {
        m_ManipFunc(Stream());
    }

    template<class T>
    void Logger::Log(const T& message) {
        std::unique_lock<std::mutex> locker(m_LoggerMutex);
        auto action = std::shared_ptr<Logger::LoggingAction>(new Logger::LogMessageAction(m_OutputStream, message));
        m_MessageQueue.push(action);
        m_MessageReady.notify_all();
    }

    void Logger::Log(manip_func_t manip_func) {
        std::unique_lock<std::mutex> locker(m_LoggerMutex);
        auto action = std::shared_ptr<Logger::LoggingAction>(new Logger::LogManipAction(m_OutputStream, manip_func));
        m_MessageQueue.push(action);
        m_MessageReady.notify_all();
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