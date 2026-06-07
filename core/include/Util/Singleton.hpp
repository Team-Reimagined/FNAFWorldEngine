#pragma once
namespace FWE::Util {
    template <typename T>
    class Singleton {
    public:
        static T *GetInstance() {
            static T instance;
            return &instance;
        }
    };
}