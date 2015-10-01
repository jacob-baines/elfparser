#include "sectionheaders.hpp"
#include "abstract_segments.hpp"
#include "abstract_sectionheader.hpp"

#include <boost/foreach.hpp>
#include <sstream>
#include <iostream>

SectionHeaders::SectionHeaders() :
    m_sectionHeaders(),
    m_totalSize(0),
    m_stringIndex(0)
{
}

SectionHeaders::~SectionHeaders()
{
}

void SectionHeaders::setHeaders(const char* p_data, const char* p_start,
                                boost::uint64_t p_total_size, boost::uint16_t p_count,
                                boost::uint16_t p_size, std::size_t p_stringIndex,
                                bool p_is64, bool p_isLE,
                                std::map<elf::Capabilties, std::set<std::string> >& p_capabilities)
{
    m_totalSize = p_total_size;
    if (p_size == 0)
    {
        return;
    }

    if (p_data > (p_start + p_total_size))
    {
        p_capabilities[elf::k_antidebug].insert("SH offset in ELF header is larger than the binary");
        return;
    }

    if (p_count > 0 && p_count <= p_stringIndex)
    {
        p_capabilities[elf::k_antidebug].insert("String index in ELF header is too large");
        return;
    }

    if (p_count > 50)
    {
        p_capabilities[elf::k_antidebug].insert("Too many sections listed in ELF header");
        return;
    }

    m_stringIndex = p_stringIndex;
    for (std::size_t i = 0; i < p_count; ++i, p_data += p_size)
    {
        if ((p_start + p_total_size) > p_data)
        {
            m_sectionHeaders.emplace_back(p_data, p_size, p_start, p_total_size,
                                          p_stringIndex, m_sectionHeaders,
                                          p_is64, p_isLE);
            if (m_sectionHeaders.back().getType() != elf::k_nobits &&
                (m_sectionHeaders.back().getPhysOffset() + m_sectionHeaders.back().getSize()) > m_totalSize)
            {
                m_sectionHeaders.pop_back();
                p_capabilities[elf::k_antidebug].insert("Invalid sections entries in section table");
            }
        }
    }
}

void SectionHeaders::extractSegments(AbstractSegments& p_segments)
{
    BOOST_FOREACH(const AbstractSectionHeader& header, m_sectionHeaders)
    {
        p_segments.makeSegmentFromSectionHeader(header);
    }
}

void SectionHeaders::evaluate(std::vector<std::pair<boost::int32_t, std::string> >& p_reasons,
                              std::map<elf::Capabilties, std::set<std::string> >& p_capabilities) const
{
    if (m_sectionHeaders.empty())
    {
        p_reasons.push_back(std::make_pair(10, std::string("Sections have been stripped or obfuscated")));
        return;
    }
}

const std::vector<AbstractSectionHeader>& SectionHeaders::getSections() const
{
    return m_sectionHeaders;
}

boost::uint32_t SectionHeaders::getStringTableIndex() const
{
    return m_stringIndex;
}

std::string SectionHeaders::printToStdOut() const
{
    std::stringstream returnValue;
    returnValue << "Section Headers (count=" << m_sectionHeaders.size()
                << ")\n";
    BOOST_FOREACH(const AbstractSectionHeader& header, m_sectionHeaders)
    {
        returnValue << "\tSection name=\"";
        returnValue <<  header.getName();
        returnValue << "\" type=" << header.getType();
        returnValue << " flags=" << header.getFlags();
        returnValue << " address=0x" << std::hex << header.getVirtAddress() << std::dec;
        returnValue << " offset=" << header.getPhysOffset();
        returnValue << " size=" << header.getSize();
        returnValue << " link=" << header.getLink();
        returnValue << " info=" << header.getInfo();
        returnValue << " align=" << header.getAddrAlign();
        returnValue << " entsize=" << header.getEntSize() << "\n";
    }
    return returnValue.str();
}
