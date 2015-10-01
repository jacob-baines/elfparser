#include "interp_segment.hpp"

#include <sstream>

InterpSegment::InterpSegment(const char* start, boost::uint32_t p_offset,
                             boost::uint32_t p_size,
                             elf::section_type p_type) :
    SegmentType(start, p_offset, p_size, p_type),
    m_interpreter(start + p_offset)
{
}

InterpSegment::~InterpSegment()
{
}

std::string InterpSegment::printToStdOut() const
{
    std::stringstream return_value;
    return_value << "Interpreter (offset=0x" << std::hex << m_offset
        << ", size=" << std::dec << m_size << ")\n"
        << "\tValue=\"" << m_interpreter << "\"\n";
    return return_value.str();
}
