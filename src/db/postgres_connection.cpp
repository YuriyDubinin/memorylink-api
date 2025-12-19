#include "postgres_connection.h"

#include <iostream>

PostgresConnection::PostgresConnection(const std::string& connection_info) {
    try {
      connection_ = std::make_unique<pqxx::connection>(connection_info);

      if (!connection_->is_open()) {
        std::cerr << "[DBConnection]: Connection failed" << std::endl;
        connection_.reset();
      }
    } catch (const std::exception& e) {
      std::cerr << "[DBConnection]: Exception while connecting: " << e.what() << std::endl;
      connection_.reset();
    }
}

void Init(std::string& connection_info) {

}

bool PostgresConnection::IsConnected() const {
    return connection_ && connection_->is_open();
}

pqxx::result PostgresConnection::execute(const std::string& query) {
    if (!IsConnected()) {
      throw std::runtime_error("[DBConnection]: is not connected");
    }

    pqxx::work tx(*connection_);
    pqxx::result res = tx.exec(query);
    tx.commit();

    return res;
}

void PostgresConnection::prepare(const std::string& name, const std::string& query) {
    if (!IsConnected()) {
      throw std::runtime_error("[DBConnection]: is not connected");
    }

    connection_->Prepare(name, query);
}

template <typename... Args>
pqxx::result PostgresConnection::execute_prepared(const std::string& name, Args&&... args) {
    if (!IsConnected()) {
      throw std::runtime_error("[DBConnection]: is not connected");
    }

    pqxx::work tx(*connection_);
    pqxx::result res = tx.exec_prepared(name, std::forward<Args>(args)...);
    tx.commit();

    return res;
}

pqxx::connection& PostgresConnection::raw() {
    if (!IsConnected()) {
      throw std::runtime_error("[DBConnection]: is not connected");
    }
    return *connection_;
}
