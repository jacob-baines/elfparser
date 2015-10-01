#include "debuglink_segment.hpp"

#include <sstream>

DebugLinkSegment::DebugLinkSegment(const char* start, boost::uint32_t p_offset,
                                   boost::uint32_t p_size,
                                   elf::section_type p_type) :
    SegmentType(start, p_offset, p_size, p_type),
    m_file(start + p_offset)
{
}

DebugLinkSegment::~DebugLinkSegment()
{
}

std::string DebugLinkSegment::printToStdOut() const
{
    std::stringstream return_value;
    return_value << "Debug Link Segment (offset=0x" << std::hex << m_offset
        << ", size=" << std::dec << m_size << ")\n"
        << "\tFile=\"" << m_file << "\"\n";
    return return_value.str();
}
