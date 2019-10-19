//
// Created by Brian on 24/09/2019.
//

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptors.hpp>
#include "logger/DefaultLogger.hpp"
#include "B12Orm.hpp"


b12software::babel::ORM::~ORM()
{
    _db.disconnect();
}

b12software::babel::ORM::ORM(const std::string &dbName)
    :   _dbName(dbName),
        _db(SQLite(dbName))
{
    _db.connect();
}

const b12software::babel::ORM &
b12software::babel::ORM::create(const std::string &table, b12software::babel::ORM::Entity fields,
                                b12software::babel::SQLite::sqliteCallback callback, bool autoQuoteValues) const
{
    std::string request = "INSERT INTO " + table + "(";

    request += boost::algorithm::join(fields | boost::adaptors::map_keys, ",");
    if (autoQuoteValues)
        request += ") VALUES(\"" + boost::algorithm::join(fields | boost::adaptors::map_values, "\",\"") + "\");";
    else
        request += ") VALUES(" + boost::algorithm::join(fields | boost::adaptors::map_values, ",") + ");";

    _db.exec(request, callback);
    return *this;
}

const b12software::babel::ORM &
b12software::babel::ORM::find(const std::string &table, b12software::babel::SQLite::sqliteCallback callback,
                                const b12software::babel::ORMParams &params, std::vector<std::string> fields) const
{
    std::string request = "SELECT ";

    if (fields.size() > 0)
        request += boost::algorithm::join(fields, ",");
    else
        request += "*";
    request += " FROM " + table;
    applyParams(request, params);
    request += ";";

    _db.exec(request, callback);
    return *this;
}

const b12software::babel::ORM &
b12software::babel::ORM::update(const std::string &table, b12software::babel::ORM::Entity fields,
                                b12software::babel::SQLite::sqliteCallback callback,
                                const b12software::babel::ORMParams &params) const
{
    std::string request = "UPDATE " + table + " SET ";

    std::vector<std::string> strfields;
    for(std::pair<std::string, std::string> field : fields)
        strfields.push_back(field.first + " = '" + field.second + "'");
    request += boost::algorithm::join(strfields, ",");
    applyParams(request, params);

    request += ";";
    DefaultLogger::Log(LogLevelDebug, "[SQLite] " + request);
    _db.exec(request, callback);
    return *this;
}

const b12software::babel::ORM &
b12software::babel::ORM::remove(const std::string &table, b12software::babel::SQLite::sqliteCallback callback,
                                const b12software::babel::ORMParams &params) const
{
    std::string request = "DELETE FROM " + table;

    applyParams(request, params);

    request += ";";
    DefaultLogger::Log(LogLevelDebug, "[SQLite] " + request);
    _db.exec(request, callback);
    return *this;
}

const b12software::babel::ORM &
b12software::babel::ORM::findById(const std::string &table, int id, b12software::babel::SQLite::sqliteCallback callback, std::vector<std::string> fields) const
{
    return find(table, callback, {{{"id", "=" + std::to_string(id)}}, 1}, fields);
}

const b12software::babel::ORM &
b12software::babel::ORM::updateById(const std::string &table, int id, b12software::babel::ORM::Entity fields,
                                    b12software::babel::SQLite::sqliteCallback callback) const
{
    return update(table, fields, callback, {{{"id", "=" + std::to_string(id)}}});
}

const b12software::babel::ORM &b12software::babel::ORM::removeById(const std::string &table, int id,
                                                                   b12software::babel::SQLite::sqliteCallback callback) const
{
    return remove(table, callback, {{{"id", "=" + std::to_string(id)}}});
}

const b12software::babel::SQLite &b12software::babel::ORM::getSQLConnection() const
{
    return _db;
}

void b12software::babel::ORM::applyParams(std::string &request, const b12software::babel::ORMParams &params) const
{
    if (params.where.size() > 0) {
        request += " WHERE ";
        std::vector<std::string> wheres;
        for(std::pair<std::string, std::string> where : params.where)
            wheres.push_back(where.first + where.second);
        request += boost::algorithm::join(wheres, " AND ");
    }
    request += params.order;
    if (params.limit >= 0)
        request += " LIMIT " + std::to_string(params.limit);
    if (params.offset >= 0)
        request += " OFFSET " + std::to_string(params.offset);
    request += params.custom;
}