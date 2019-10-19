//
// Created by Romain FOUYER on 2019-09-27.
//

#ifndef BABEL_BABELCLIENTUSER_HPP
#define BABEL_BABELCLIENTUSER_HPP

#include <string>

namespace b12software {

    namespace babel {

        class BabelClientUser {
        public:
            BabelClientUser(const std::string &userName, const std::string &avatarPath, const std::string &emailAddress = "", long id = 99999);
            ~BabelClientUser() = default;

            void setUsername(const std::string &newUsername);
            void setAvatarPath(const std::string &newAvatarPath);
            void setEmailAddress(const std::string &newEmailAddress);
            void setId(long id);
            void setInCall(bool newState);

            [[nodiscard]] bool isInCall() const;
            [[nodiscard]] const std::string &getUsername() const;
            [[nodiscard]] const std::string &getAvatarPath() const;
            [[nodiscard]] const std::string &getEmailAddress() const;
            [[nodiscard]] long getId() const;
        private:
            std::string _userName;
            std::string _avatarPath;
            std::string _emailAddress;
            long _id;
            bool _inCall;
        };

    }

}


#endif //BABEL_BABELCLIENTUSER_HPP
