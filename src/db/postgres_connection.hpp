#pragma once

#include <iostream>
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
    void         prepare(const std::string& name, const std::string& query);

    template <typename... Args>
    pqxx::result execute_prepared(const std::string& name, Args&&... args) {
        if (!IsConnected())
            throw std::runtime_error("[DBConnection]: is not connected");

        pqxx::work tx(*connection_);

        pqxx::result res = tx.exec(pqxx::prepped{name}, pqxx::params{std::forward<Args>(args)...});

        tx.commit();
        return res;
    }

    pqxx::connection& raw();

private:
    std::unique_ptr<pqxx::connection> connection_;
};
