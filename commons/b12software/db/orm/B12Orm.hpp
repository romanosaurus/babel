//
// Created by Brian on 24/09/2019.
//

#ifndef BABEL_B12ORM_HPP
#define BABEL_B12ORM_HPP

#include <vector>

#include "b12software/db/SQLite.hpp"

/*!
 * @namespace b12software
 * @brief Main namespace for all b12 software
 */

namespace b12software {

    /*!
     * @namespace b12software::babel
     * @brief Main namespace for the babel project
     */
    namespace babel {

        /*!
         * @struct ORMParams
         * @brief a struct for all ORM params
         */
        struct ORMParams {
            std::map<std::string, std::string> where;
            int limit = -1;
            int offset = -1;
            std::string order = "";
            std::string custom = "";
        };

        /*!
         * @class ORM
         * @brief A little personnal orm
         */
        class ORM {
        public:

            typedef std::map<std::string, std::string> Entity;

            /*!
             * @brief dtor
             */
            ~ORM();

            /*!
             * @brief ctor
             * @param dbName
             */
            ORM(const std::string &dbName);

            /*!
             * @brief Create an element in this table
             * @param table
             * @param fields
             * @param callback
             * @return
             */
            const ORM &create(const std::string &table, Entity fields, SQLite::sqliteCallback callback = SQLite::sqliteCallback(), bool autoQuoteValues = true) const;

            /*!
             * @brief Find some element in a table
             * @param table
             * @param params
             * @param callback
             * @return
             */
            const ORM &find(const std::string &table, SQLite::sqliteCallback callback = SQLite::sqliteCallback(), const ORMParams &params = {}, std::vector<std::string> fields = {}) const;

            /*!
             * @brief update an element in a table
             * @param table
             * @param params
             * @param callback
             * @return
             */
            const ORM &update(const std::string &table, Entity fields, SQLite::sqliteCallback callback = SQLite::sqliteCallback(), const ORMParams &params = {}) const;

            /*!
             * @brief Delete some element in a table
             * @param table
             * @param params
             * @param callback
             * @return
             */
            const ORM &remove(const std::string &table, SQLite::sqliteCallback callback = SQLite::sqliteCallback(), const ORMParams &params = {}) const;

            /*!
             * @brief Find element in table by id
             * @param table
             * @param id
             * @param callback
             * @return
             */
            const ORM &findById(const std::string &table, int id, SQLite::sqliteCallback callback = SQLite::sqliteCallback(), std::vector<std::string> fields = {}) const;

            /*!
             * @brief Update element in table by id
             * @param table
             * @param id
             * @param callback
             * @return
             */
            const ORM &updateById(const std::string &table, int id, Entity fields, SQLite::sqliteCallback callback = SQLite::sqliteCallback()) const;

            /*!
             * @brief Delete element in table by id
             * @param table
             * @param id
             * @param callback
             * @return
             */
            const ORM &removeById(const std::string &table, int id, SQLite::sqliteCallback callback = SQLite::sqliteCallback()) const;

            /*!
             * @brief return SQLite connection
             * @return
             */
            const SQLite &getSQLConnection() const;

        private:
            /*!
             * @brief apply ORMParams to request
             * @param request
             * @param params
             */
            void applyParams(std::string &request, const ORMParams &params) const;

        private:
            std::string _dbName;/*!< Name of the database */
            SQLite _db;/*!< SQLite object */
        };
    }
};


#endif //BABEL_B12ORM_HPP
