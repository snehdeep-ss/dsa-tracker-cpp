
#include "TCPServer.h"
#include "EpollEventLoop.h"
#include "../infra/Config.h"
#include "../infra/Logger.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

TCPServer::TCPServer(EpollEventLoop &eventLoopPtr,
                     std::shared_ptr<Config> config) : m_eventLoop(eventLoopPtr), m_config(std::move(config))
{
    // Logger::info("TCPServer initialized");
}

TCPServer::~TCPServer()
{
    stop();
    // Logger::info("TCPServer destroyed");
}

std::optional<TCPServer::ServerError> TCPServer::start()
{
    if (m_isRunning)
    {
        return ServerError{"Server is already running", -1};
    }
    m_serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (m_serverFd == -1)
    {
        // Logger::error("Failed to create server socket");
        return ServerError{"Failed to create server socket", errno};
    }
    int opt = 1;

    setsockopt(m_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(m_serverFd, (struct sockaddr *)&m_config->serverAddress, sizeof(m_config->serverAddress)) < 0)
    {
        int saved_errno = errno;
        int port = ntohs(m_config->serverAddress.sin_port);
        close(m_serverFd);
        m_serverFd = -1;
        // Logger::error("Failed to bind server socket");
        return ServerError{"Failed to bind server socket" + std::to_string(port), saved_errno};
    }

    if (listen(m_serverFd, SOMAXCONN) < 0)
    {
        close(m_serverFd);
        m_serverFd = -1;
        // Logger::error("Failed to listen on server socket");
        return ServerError{"Failed to listen on server socket", errno};
    }

    m_isRunning = true;

    m_acceptThread = std::thread(&TCPServer::acceptLoop, this);

    return std::nullopt;
}

void TCPServer::stop()
{
    if (m_isRunning)
    {
        m_isRunning = false;
        close(m_serverFd);
        m_serverFd = -1;
        if (m_acceptThread.joinable())
            m_acceptThread.join();
        // Logger::info("TCPServer stopped");
    }
}

void TCPServer::acceptLoop()
{
    while (m_isRunning)
    {
        int clientFd = accept(m_serverFd, nullptr, nullptr);
        if (clientFd == -1)
        {
            if (errno == EINTR)
                continue;
            if (!m_isRunning)
                break;
            // Logger::error("Failed to accept connection");
            continue;
        }
        passToEventLoop(clientFd);
    }
}

void TCPServer::passToEventLoop(int fd)
{
    try
    {
        m_eventLoop.addClient(fd);
        return ServerError{"", 0};
    }
    catch (const std::exception &e)
    {
        close(fd);
        // Logger::error("Failed to add client to event loop: " + std::string(e.what()));
        return ServerError{"Failed to add client to event loop: " + std::string(e.what()), -1};
    }
}
