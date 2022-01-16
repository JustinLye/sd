#include "sd/framework/logging/logger.h"

#include <fstream>

namespace sd {
namespace framework {
namespace logging {

    Logger::Logger() :
        m_OutputStream(std::cout),
        m_LoggerMutex(),
        m_MessageQueue(),
        m_MessageReady(),
        m_Stop(false),
        m_LoggingLoop(),
        m_FileStream(nullptr) {
        m_LoggingLoop = std::move(std::async(std::launch::async, &Logger::LoggingLoop, std::ref(*this)));
    }

    Logger::Logger(const std::filesystem::path& file_path) :
        m_OutputStream(std::cout),
        m_LoggerMutex(),
        m_MessageQueue(),
        m_MessageReady(),
        m_Stop(false),
        m_LoggingLoop(),
        m_FileStream(std::make_shared<std::ofstream>(file_path)) {
        m_OutputStream.set_rdbuf(m_FileStream->rdbuf());
        m_LoggingLoop = std::move(std::async(std::launch::async, &Logger::LoggingLoop, std::ref(*this)));
    }

    Logger::~Logger() {
        try {
            m_Stop = true;
            m_MessageReady.notify_all();
            m_LoggingLoop.wait();
            if (m_FileStream) {
                m_FileStream->close();
            }
        } catch (...) {}
    }

    void Logger::LoggingLoop() {
        while (!m_Stop) {
            auto action = GetNextLoggingAction();
            if (action) {
                action->Log();
            }
        }
        {
            std::lock_guard<std::mutex> locker(m_LoggerMutex);
            while (!m_MessageQueue.empty()) {
                auto action = m_MessageQueue.front();
                m_MessageQueue.pop();
                if (action) {
                    action->Log();
                }
            }
        }
    }

    std::shared_ptr<Logger::LoggingAction> Logger::GetNextLoggingAction() {
        std::unique_lock<std::mutex> locker(m_LoggerMutex);
        while (!m_Stop && m_MessageQueue.empty()) {
            m_MessageReady.wait(locker);
        }
        if (!m_MessageQueue.empty()) {
            auto result = m_MessageQueue.front();
            m_MessageQueue.pop();
            return result;
        }
        return {};
    }


}}}