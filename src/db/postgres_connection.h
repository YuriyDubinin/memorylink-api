#pragma once

#include <memory>
#include <string>

#include <pqxx/pqxx>

class PostgresConnection {
public:
    explicit PostgresConnection(const std::string& connection_info);
    ~PostgresConnection() = default;

    PostgresConnection(const PostgresConnection&)            = delete;
    PostgresConnection& operator=(const PostgresConnection&) = delete;

    void Check() const;

    bool IsConnected() const;

    pqxx::result execute(const std::string& query);

    void prepare(const std::string& name, const std::string& query);

    template <typename... Args>
    pqxx::result execute_prepared(const std::string& name, Args&&... args);

    pqxx::connection& raw();

private:
    std::unique_ptr<pqxx::connection> connection_;
};
