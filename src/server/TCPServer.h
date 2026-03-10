#pragma once
/* TCPServer
 * Responsibility: Own the server socket lifecycle — bind, listen,
 *                 accept incoming connections, and hand off each
 *                 new connection fd to EpollEventLoop.
 *                 Does NOT read data or parse HTTP.
 */

#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <atomic>

class EpollEventLoop;
class Config;

class TCPServer
{
public:
    TCPServer(EpollEventLoop &eventLoopPtr, std::shared_ptr<Config> config);
    ~TCPServer();

    TCPServer(const TCPServer &) = delete;
    TCPServer &operator=(const TCPServer &) = delete;

    TCPServer(TCPServer &&) = delete;
    TCPServer &operator=(TCPServer &&) = delete;

    struct ServerError
    {
        std::string message;
        int errorCode;
    };

    std::optional<ServerError> start();

    void stop();

private:
    void acceptLoop();
    void passToEventLoop(int fd);

private:
    int m_serverFd = -1;
    std::atomic<bool> m_isRunning{false};
    std::thread m_acceptThread;

    EpollEventLoop &m_eventLoop;
    std::shared_ptr<Config> m_config;
};
