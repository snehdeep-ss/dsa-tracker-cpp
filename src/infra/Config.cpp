#include "Config.h"
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <iostream>

std::string Config::require(const std::string &key)
{
    const char *value = std::getenv(key.c_str());
    if (!value)
    {
        throw std::runtime_error("Required config key not found: " + key);
    }
    return std::string(value);
}

std::string Config::optional(const std::string &key, const std::string &defaultValue)
{
    const char *value = std::getenv(key.c_str());
    if (!value)
    {
        return defaultValue;
    }
    return std::string(value);
}

int Config::optionalInt(const std::string &key, int defaultValue)
{
    const char *value = std::getenv(key.c_str());
    if (!value)
    {
        return defaultValue;
    }
    try
    {
        return std::stoi(std::string(value));
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Invalid integer for config key: " + key);
    }
}

static std::string trim(const std::string &str)
{
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";
    return str.substr(start, end - start + 1);
}

static void loadEnvironmentVariables(const std::string &path)
{
    std::ifstream envFile(path);

    if (envFile.is_open())
    {
        std::string line;
        while (std::getline(envFile, line))
        {
            if (line.empty() || line[0] == '#')
                continue;
            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos)
            {
                std::string key = trim(line.substr(0, delimiterPos));
                std::string value = trim(line.substr(delimiterPos + 1));
                setenv(key.c_str(), value.c_str(), 0);
            }
        }
        envFile.close();
    }
    else
    {
        std::cerr << "Could not open environment file: " << path << "\n";
    }
}

Config &Config::getInstance()
{
    static Config instance;
    return instance;
}

void Config::initialize(const std::string &envFilePath)
{
    loadEnvironmentVariables(envFilePath);
    Config::getInstance(); // Force initialization of the singleton instance
}

Config::Config() : m_githubOAuthConfig(Config::loadGithubOAuthConfig()),
                   m_loggingConfig(Config::loadLoggingConfig()),
                   m_dbConfig(Config::loadDBConfig()),
                   m_serverConfig(Config::loadServerConfig()),
                   m_aiConfig(Config::loadAIConfig()),
                   m_jwtConfig(Config::loadJWTConfig()),
                   m_rateLimitConfig(Config::loadRateLimitConfig()),
                   m_encryptionConfig(Config::loadEncryptionConfig())
{
}

Config::~Config() {}

Config::GithubOAuthConfig Config::loadGithubOAuthConfig()
{
    // Load from file, environment variables, or hardcoded values
    return GithubOAuthConfig{
        .githubClientId = require("GITHUB_CLIENT_ID"),
        .githubClientSecret = require("GITHUB_CLIENT_SECRET"),
        .githubRedirectUri = optional("GITHUB_REDIRECT_URI", "http://localhost:8080/auth/github/callback")};
}

Config::LoggingConfig Config::loadLoggingConfig()
{
    return LoggingConfig{
        .logLevel = optional("LOG_LEVEL", "INFO"),
        .logFilePath = optional("LOG_FILE_PATH", "logs/app.log")};
}

Config::DBConfig Config::loadDBConfig()
{
    return DBConfig{
        .path = optional("DB_PATH", "./dsa-tracker.db")};
}

Config::ServerConfig Config::loadServerConfig()
{
    return ServerConfig{
        .port = optionalInt("SERVER_PORT", 8080),
        .maxConnections = optionalInt("MAX_CONNECTIONS", 1000),
        .threadPoolSize = optionalInt("THREAD_POOL_SIZE", 4)};
}

Config::AIConfig Config::loadAIConfig()
{
    return AIConfig{
        .apiKey = require("AI_API_KEY"),
        .model = optional("AI_MODEL", "gpt-3.5-turbo"),
        .apiUrl = optional("AI_API_URL", "AI LINK HERE")};
}

Config::JWTConfig Config::loadJWTConfig()
{
    return JWTConfig{
        .secretKey = require("JWT_SECRET_KEY"),
        .expirationSeconds = optionalInt("JWT_EXPIRATION_SECONDS", 3600)};
}

Config::RateLimitConfig Config::loadRateLimitConfig()
{
    return RateLimitConfig{
        .maxRequestsPerMinute = optionalInt("MAX_REQUESTS_PER_MINUTE", 60)};
}

Config::EncryptionConfig Config::loadEncryptionConfig()
{
    return EncryptionConfig{
        .encryptionKey = require("ENCRYPTION_KEY")};
}
