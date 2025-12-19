#pragma once

#include <memory>
#include <string>
#include <pqxx/pqxx>

class PostgresConnection {
public:
    explicit PostgresConnection(const std::string& connection_info);
    ~PostgresConnection() = default;

    PostgresConnection(const PostgresConnection&) = delete;
    PostgresConnection& operator=(const PostgresConnection&) = delete;

    void Init(std::string& connection_info);

    bool IsConnected() const;

    pqxx::result Execute(const std::string& query);

    void Prepare(const std::string& name, const std::string& query);

    template <typename... Args>
    pqxx::result execute_prepared(const std::string& name, Args&&... args);

    pqxx::connection& Raw();

private:
    std::unique_ptr<pqxx::connection> connection_;
};
