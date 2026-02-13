#pragma once

#include "vbase/core/assert.hpp"
#include "vbase/module/imodule.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace vbase
{
    class ModuleRegistry
    {
    public:
        struct Node
        {
            IModule*                 mod {};
            std::vector<std::string> deps;
        };

        void add(IModule& mod, std::vector<std::string> deps = {})
        {
            const std::string n = mod.name();
            VBASE_ASSERT(!n.empty());
            VBASE_ASSERT(m_NodesByName.find(n) == m_NodesByName.end());

            m_Nodes.push_back(Node {&mod, std::move(deps)});
            m_NodesByName[n] = static_cast<uint32_t>(m_Nodes.size()) - 1;
        }

        bool initAll()
        {
            m_InitOrder.clear();
            m_State.clear();

            for (auto& n : m_Nodes)
            {
                if (!dfsVisit(n.mod->name()))
                    return false;
            }

            for (auto idx : m_InitOrder)
            {
                if (!m_Nodes[idx].mod->init())
                {
                    shutdownAll(); // rollback already-inited
                    return false;
                }
            }
            m_Inited = true;
            return true;
        }

        void shutdownAll()
        {
            if (!m_Inited)
                return;

            for (size_t i = m_InitOrder.size(); i > 0; --i)
            {
                auto idx = m_InitOrder[i - 1];
                m_Nodes[idx].mod->shutdown();
            }
            m_Inited = false;
        }

    private:
        enum class VisitState : uint8_t
        {
            eUnvisited,
            eVisiting,
            eVisited
        };

        bool dfsVisit(const std::string& name)
        {
            auto stIt = m_State.find(name);
            if (stIt != m_State.end())
            {
                if (stIt->second == VisitState::eVisiting)
                    return false; // cycle
                if (stIt->second == VisitState::eVisited)
                    return true;
            }

            m_State[name] = VisitState::eVisiting;

            auto it = m_NodesByName.find(name);
            VBASE_ASSERT(it != m_NodesByName.end());
            const auto& node = m_Nodes[it->second];

            for (const auto& dep : node.deps)
            {
                if (m_NodesByName.find(dep) == m_NodesByName.end())
                    return false; // missing dep
                if (!dfsVisit(dep))
                    return false;
            }

            m_State[name] = VisitState::eVisited;
            m_InitOrder.push_back(it->second);
            return true;
        }

        bool                                      m_Inited {false};
        std::vector<Node>                         m_Nodes;
        std::unordered_map<std::string, uint32_t> m_NodesByName;

        std::unordered_map<std::string, VisitState> m_State;
        std::vector<uint32_t>                       m_InitOrder;
    };
} // namespace vbase
