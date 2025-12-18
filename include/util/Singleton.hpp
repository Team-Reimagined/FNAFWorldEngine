#pragma once

namespace FNAFWorldEngine {
    namespace Utils {
        template <typename Derived>
        class Singleton {
        public:
            static Derived* get() {
                static Derived instance;
                return &instance;
            }
        };
    }
}