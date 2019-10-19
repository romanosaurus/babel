/*
** EPITECH PROJECT, 2019
** Babel
** File description:
** ORMContainer.hpp
*/

/* Created the 18/09/2019 at 15:38 by julian.frabel@epitech.eu */

#ifndef BABEL_ORMCONTAINER_HPP
#define BABEL_ORMCONTAINER_HPP

#include "b12software/db/orm/B12Orm.hpp"

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
         * @class DefaultORM
         * @brief A static class used to set and use a default logger
         */
        class ORMContainer {
        private:
            /*!
             * @brief You should not be able to instantiate a ORMContainer class
             */
            ORMContainer() = default;
        public:
            /*!
             * @brief Set the ORM to use as default
             * @param orm the ORM to use
             */
            static void SetDefaultORM(ORM *orm);

            /*!
             * @brief set the ORM to use as default (create a new ORM)
             * @param dbName
             */
            static void SetDefaultORM(const std::string &dbName);

            /*!
             * @brief Get the current ORM
             * @return The current used ORM
             */
            static ORM *GetCurrentORM();

            /*!
             * @brief Delete current default ORM
             */
            static void CleanORM();

        private:
            static ORM *currentORM; /*!< The current ORM to use */
        };
    }
}

#endif //BABEL_ORMCONTAINER_HPP
