#ifndef SEARCH_VALUE_HPP
#define SEARCH_VALUE_HPP

#include <string>

#include "../structures/capabilities.hpp"

class SearchValue
{
public:

    SearchValue(const std::string& p_search, elf::Capabilties p_capabilities,
                const std::string& p_info) :
        m_search(p_search),
        m_type(p_capabilities),
        m_info(p_info)
    {
    }

    ~SearchValue()
    {
    }

public:
    std::string m_search;
    elf::Capabilties m_type;
    std::string m_info;
};

#endif
