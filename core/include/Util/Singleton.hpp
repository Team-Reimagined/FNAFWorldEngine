#pragma once
namespace FWE::Util {
    template <typename T>
    class Singleton {
    public:
        static T Get() {
            static T instance;
            return instance;
        }
    };
}