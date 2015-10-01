#include "dynamicsection.hpp"
#include "structures/dynamicstruct.hpp"
#include "abstract_segments.hpp"

#include <boost/foreach.hpp>
#include <boost/assign.hpp>

#ifdef __APPLE__
#include "endian.h"
#elif WINDOWS
#include "endian.h"
#else
#include <arpa/inet.h>
#endif

DynamicSection::DynamicSection() :
    m_offset(0),
    m_symbolTableVirtAddress(0),
    m_stringTableVirtAddress(0),
    m_stringTableSize(0),
    m_symbolTableSize(0),
    m_initArrayVirtAddress(0),
    m_initArrayEntries(0),
    m_entries()
{

}

DynamicSection::~DynamicSection()
{
}

boost::uint64_t DynamicSection::getOffset() const
{
    return m_offset;
}

void DynamicSection::createDynamic(const char* p_start, boost::uint32_t p_offset,
                                   boost::uint32_t p_size, boost::uint64_t p_baseAddress,
                                   bool p_is64, bool p_isLE, const AbstractSegments& p_segments)
{
    if (p_offset == 0)
    {
        return;
    }
    else
    {
        m_offset = p_offset;
    }

    // create all the entries
    if (p_is64)
    {
        doDynamic64(reinterpret_cast<const elf::dynamic::dynamic_64*>(p_start + p_offset),
                    p_start, p_start + p_offset + p_size, p_baseAddress, p_isLE);
    }
    else
    {
        doDynamic32(reinterpret_cast<const elf::dynamic::dynamic_32*>(p_start + p_offset),
                    p_start, p_start + p_offset + p_size, p_baseAddress, p_isLE);
    }

    // look for strtab
    std::size_t i = 0;
    for ( ; i < m_entries.size(); ++i)
    {
        if (m_entries[i].getTag() == elf::dynamic::k_strtab)
        {
            break;
        }
    }

    // resolve all strings
    if (i != m_entries.size())
    {
        boost::uint64_t offset = p_segments.getOffsetFromVirt(m_entries[i].getValue());
        if (offset != 0)
        {
            BOOST_FOREACH(AbstractDynamicEntry& entry, m_entries)
            {
                entry.createString(p_start + offset);
            }
        }
    }

    // look for symbol table info
    BOOST_FOREACH(const AbstractDynamicEntry& entry, m_entries)
    {
        switch (entry.getTag())
        {
            case elf::dynamic::k_symtab:
                m_symbolTableVirtAddress = entry.getValue();
                break;
            case elf::dynamic::k_strtab:
                m_stringTableVirtAddress = entry.getValue();
                break;
            case elf::dynamic::k_strsz:
                m_stringTableSize = entry.getValue();
                break;
            case elf::dynamic::k_hash:
            case elf::dynamic::k_gnuhash:
                if (m_symbolTableSize == 0)
                {
                    const boost::uint32_t* hashStart =
                        reinterpret_cast<const boost::uint32_t*>(p_start + (entry.getValue() - p_baseAddress));
                    ++hashStart;
                    m_symbolTableSize = *hashStart;
                    if (!p_isLE)
                    {
                        m_symbolTableSize = ntohl(m_symbolTableSize);
                    }
                }
                break;
            case elf::dynamic::k_initarray:
                m_initArrayVirtAddress = entry.getValue();
                break;
            case elf::dynamic::k_init_arraysz:
                m_initArrayEntries = entry.getValue() / (p_is64 ? 8 : 4);
                break;
            default:
                break;
        }
    }
}

void DynamicSection::doDynamic64(const elf::dynamic::dynamic_64* p_dynamic,
                                 const char* p_start, const char* p_end,
                                 boost::uint64_t p_baseAddress, bool p_isLE)
{
    for ( ; reinterpret_cast<const char*>(p_dynamic) < p_end; ++p_dynamic)
    {
        boost::uint64_t tag = p_isLE ? p_dynamic->m_tag : htobe64(p_dynamic->m_tag);
        boost::uint64_t value = p_isLE ? p_dynamic->m_val : htobe64(p_dynamic->m_val);
        m_entries.emplace_back(tag, value);

        if (tag == 0)
        {
            return;
        }
    }
}

void DynamicSection::doDynamic32 (const elf::dynamic::dynamic_32* p_dynamic,
                                  const char* p_start, const char* p_end,
                                  boost::uint64_t p_baseAddress, bool p_isLE)
{
    for ( ; reinterpret_cast<const char*>(p_dynamic) < p_end; ++p_dynamic)
    {
        boost::uint32_t tag = p_isLE ? p_dynamic->m_tag : ntohl(p_dynamic->m_tag);
        boost::uint32_t value = p_isLE ? p_dynamic->m_val : ntohl(p_dynamic->m_val);
        m_entries.emplace_back(tag, value);

        if (tag == 0)
        {
            return;
        }
    }
}

boost::uint64_t DynamicSection::getSymbolTableVirtAddress() const
{
    return m_symbolTableVirtAddress;
}

boost::uint64_t DynamicSection::getStringTableVirtualAddress() const
{
    return m_stringTableVirtAddress;
}

boost::uint64_t DynamicSection::getStringTableSize() const
{
    return m_stringTableSize;
}

boost::uint32_t DynamicSection::getSymbolTableSize() const
{
    return m_symbolTableSize;
}

boost::uint64_t DynamicSection::getInitArray() const
{
    return m_initArrayVirtAddress;
}

boost::uint32_t DynamicSection::getInitArrayEntries() const
{
    return m_initArrayEntries;
}

void DynamicSection::evaluate(std::vector<std::pair<boost::int32_t, std::string> >& p_reasons,
                              std::map<elf::Capabilties, std::set<std::string> >& p_capabilities) const
{
    std::set<std::string> needed;
    BOOST_FOREACH(const AbstractDynamicEntry& entry, m_entries)
    {
        if (entry.getTag() == elf::dynamic::k_needed)
        {
            needed.insert(entry.getString());
        }
    }

    if (!m_entries.empty() && needed.empty())
    {
        p_reasons.push_back(std::make_pair(10, std::string("Contains dynamic table with no needed shared objects.")));
    }
}

std::string DynamicSection::printToStdOut() const
{
    std::stringstream returnValue;
    returnValue << "Dynamic Section (count = " << m_entries.size() << ")\n";
    BOOST_FOREACH(const AbstractDynamicEntry& entry, m_entries)
    {
        returnValue << entry.printToStdOut();
    }
    return returnValue.str();
}
