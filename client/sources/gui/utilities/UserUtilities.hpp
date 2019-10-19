
/* Created by Romain FOUYER on 03/10/2019 at 17:41. */


#ifndef BABEL_USERUTILITIES_HPP
#define BABEL_USERUTILITIES_HPP

#include "gui/BabelClientUser.hpp"

#include <tuple>

namespace b12software {
    namespace babel {
        class UserUtilities {
        public:
            static UserUtilities &getInstance();

            void setCalledId(long newId);
            [[nodiscard]] long getCalledId() const;

            void setCalledUser(BabelClientUser &user);
            BabelClientUser &getCalledUser();

            void setCommunicationInfo(int ipAddr, int port);
            std::tuple<int, int> &getCommunicationInfo() const;

            void setWaitingCall(bool newStatus);
            bool getWaitingCall() const;

        private:
            UserUtilities();
            UserUtilities(const UserUtilities &other) = delete;
            UserUtilities(UserUtilities &&other) = delete;

        private:
            long _calledId;
            BabelClientUser _calledUser;
            std::tuple<int, int> _communicationInfo;
            bool _waitingCall;
        };
    }
}


#endif //BABEL_USERUTILITIES_HPP
