
/* Created by Romain FOUYER on 2019-10-01 at 19:36. */


#ifndef BABEL_ASSETSMANAGER_HPP
#define BABEL_ASSETSMANAGER_HPP

#include <map>
#include <string>

namespace b12software {

    namespace babel {

        class AssetsManager {
        public:
            static AssetsManager &getInstance();

        private:
            AssetsManager();
            AssetsManager(const AssetsManager &other) = delete;
            AssetsManager(AssetsManager &&other) = delete;

        public:
            std::map<std::string, std::string> getAssets() const;
            static std::string getAsset(const std::string &asset);

        private:
            std::map <std::string, std::string> _assets;

        public:
            const std::string &operator[](const std::string &asset);
        };

    }

}

#endif //BABEL_ASSETSMANAGER_HPP
