#pragma once
// Config
// Responsibility: Load all environment variables once at startup
//                 via initialize(). Provide structured, read-only
//                 access to application configuration grouped by
//                 domain (server, db, jwt, ai, github, encryption).
//                 Fail fast at startup if required variables are missing.
//                 Singleton — one instance for the lifetime of the app.
//                 Does NOT perform I/O after construction.
//                 Does NOT store user credentials — only app-level config.

#include <string>

class Config
{

public:
    struct GithubOAuthConfig
    {
        std::string githubClientId;
        std::string githubClientSecret;
        std::string githubRedirectUri;
    };

    struct DBConfig
    {
        std::string path;
    };

    struct AIConfig
    {
        std::string apiKey;
        std::string model;
        std::string apiUrl;
    };

    struct ServerConfig
    {
        int port;
        int maxConnections;
        int threadPoolSize;
    };

    struct JWTConfig
    {
        std::string secretKey;
        int expirationSeconds;
    };

    struct RateLimitConfig
    {
        int maxRequestsPerMinute;
    };

    struct LoggingConfig
    {
        std::string logLevel;
        std::string logFilePath;
    };

    struct EncryptionConfig
    {
        std::string encryptionKey;
    };

    const GithubOAuthConfig m_githubOAuthConfig;
    const LoggingConfig m_loggingConfig;
    const DBConfig m_dbConfig;
    const ServerConfig m_serverConfig;
    const AIConfig m_aiConfig;
    const JWTConfig m_jwtConfig;
    const RateLimitConfig m_rateLimitConfig;
    const EncryptionConfig m_encryptionConfig;

private:
    Config();

    static GithubOAuthConfig loadGithubOAuthConfig();
    static LoggingConfig loadLoggingConfig();
    static DBConfig loadDBConfig();
    static ServerConfig loadServerConfig();
    static AIConfig loadAIConfig();
    static JWTConfig loadJWTConfig();
    static RateLimitConfig loadRateLimitConfig();
    static EncryptionConfig loadEncryptionConfig();

    static std::string require(const std::string &key);
    static std::string optional(const std::string &key, const std::string &defaultValue);
    static int optionalInt(const std::string &key, int defaultValue);

public:
    static void initialize(const std::string &envFilePath);
    static Config &getInstance();

    Config(const Config &) = delete;
    Config &operator=(const Config &) = delete;
    Config(Config &&) = delete;
    Config &operator=(Config &&) = delete;

    ~Config();
};
