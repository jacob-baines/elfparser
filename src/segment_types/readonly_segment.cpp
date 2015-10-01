#include "readonly_segment.hpp"

#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <sstream>

ReadOnlySegment::ReadOnlySegment(const char* start, boost::uint32_t p_offset,
                                 boost::uint32_t p_size, elf::section_type p_type) :
    SegmentType(start, p_offset, p_size, p_type),
    m_asciiStrings()
{
    std::string current;
    const char* readOnly = start + p_offset;
    for (boost::uint32_t i = 0; i < p_size; ++i)
    {
        if (isprint(static_cast<boost::uint8_t>(readOnly[i])))
        {
            current.push_back(readOnly[i]);
        }
        else
        {
            if (current.size() > 7)
            {
                m_asciiStrings.insert(current);
            }
            current.clear();
        }
    }
}

ReadOnlySegment::~ReadOnlySegment()
{
}

std::string ReadOnlySegment::printToStdOut() const
{
    std::stringstream return_value;
    return_value << "Read Only Segment (offset=0x" << std::hex << m_offset
                 << ", size=" << std::dec << m_size << ", strings="
                 << m_asciiStrings.size() << ")\n";

    BOOST_FOREACH(const std::string& p_ascii, m_asciiStrings)
    {
        return_value << "\tString=\"" << p_ascii << "\"\n";
    }
    return return_value.str();
}

void ReadOnlySegment::evaluate(std::vector<std::pair<boost::int32_t, std::string> >& p_reasons,
                               std::map<elf::Capabilties, std::set<std::string> >&) const
{
    if (m_asciiStrings.empty())
    {
        p_reasons.push_back(std::make_pair(5, std::string("No ascii strings in the read only section.")));
    }
}
