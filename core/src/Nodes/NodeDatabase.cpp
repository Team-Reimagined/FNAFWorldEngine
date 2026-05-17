#include "Nodes/NodeDatabase.hpp"
#include <type_traits>
#include "Util/Logging.hpp"

namespace FWE::Nodes
{
    NodeDatabase *NodeDatabase::GetInstance()
    {
        static NodeDatabase nodeDatabase;
        return &nodeDatabase;
    }

    template<class T> void NodeDatabase::Register(const char *name)
    {
        if(std::is_base_of_v<Node, T>)
        {
            database.insert({name, std::make_unique<T>()});
        }
        else
        {
            Util::Logging::error("Unable to register class. Class is not derived from Node.");
        }
    }
}