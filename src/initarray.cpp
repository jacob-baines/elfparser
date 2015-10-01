#include "initarray.hpp"

#include <sstream>
#include <boost/foreach.hpp>

#ifdef __APPLE__
#include "endian.h"
#elif WINDOWS
#include "endian.h"
#else
#include <arpa/inet.h>
#endif

InitArray::InitArray(const std::string& p_name) :
    m_name(p_name),
    m_offset(0),
    m_entries()
{
}

InitArray::~InitArray()
{
}

void InitArray::set(const char* p_data, boost::uint32_t p_size,
                    boost::uint32_t p_offset, boost::uint32_t p_entries,
                    bool is64, bool isLE)
{
    const char* offset = p_data + p_offset;
    const char* end = p_data + p_offset;
    m_offset = p_offset;
    boost::uint32_t size = 4;
    if (is64)
    {
        size = 8;
    }
    end += (size * p_entries);

    for ( ; offset < end; offset += size)
    {
        if (is64)
        {
            boost::uint64_t address = *reinterpret_cast<const boost::uint64_t*>(offset);
            if (!isLE)
            {
                address = htobe64(address);
            }
            m_entries.push_back(std::make_pair(address, ""));
        }
        else
        {
            boost::uint32_t address = *reinterpret_cast<const boost::uint32_t*>(offset);
            if (!isLE)
            {
                address = ntohl(address);
            }
            m_entries.push_back(std::make_pair(address, ""));
        }
    }
}

boost::uint32_t InitArray::getOffset() const
{
    return m_offset;
}

std::vector<std::pair<boost::uint64_t, std::string> >& InitArray::getEntries()
{
    return m_entries;
}

std::string InitArray::printToStd() const
{
    std::stringstream returnValue;
    returnValue << m_name << " (" << m_entries.size() << ")" << std::endl;
    for (std::size_t i = 0; i < m_entries.size(); ++i)
    {
        returnValue << "\tEntry addess=0x" << std::hex << m_entries[i].first << " name=" << m_entries[i].second << std::endl;
    }

    return returnValue.str();
}
