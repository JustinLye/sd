#include "sd/framework/logging/logger.h"

#include <fstream>

namespace sd {
namespace framework {
namespace logging {

    Logger::Logger() :
        m_Channels(),
        m_LoggerMutex(),
        m_MessageQueue(),
        m_MessageReady(),
        m_Stop(false),
        m_LoggingLoop(),
        m_LogLevel(log_level_t::info) {
        m_LoggingLoop = std::move(std::async(std::launch::async, &Logger::LoggingLoop, std::ref(*this)));
    }

    Logger::~Logger() {
        try {
            m_Stop = true;
            m_MessageReady.notify_all();
            m_LoggingLoop.wait();
            for (auto channel : m_Channels) {
                if (channel.stream) {
                    auto file_stream = dynamic_cast<std::ofstream*>(channel.stream.get());
                    if (file_stream) {
                        file_stream->close();
                    }
                }

            }

        } catch (...) {}
    }

    void Logger::AddChannel(const std::ostream& stream, log_level_t level) {
        m_Channels.push_back({ level, std::make_shared<std::ostream>(stream.rdbuf())});
    }

    void Logger::AddChannel(const std::filesystem::path& path, log_level_t level) {
        m_Channels.push_back({ level, std::make_shared<std::ofstream>(path)});
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