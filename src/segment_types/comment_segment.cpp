#include "comment_segment.hpp"

#include <sstream>

CommentSegment::CommentSegment(const char* start, boost::uint32_t p_offset,
                               boost::uint32_t p_size,
                               elf::section_type p_type) :
                               SegmentType(start, p_offset, p_size, p_type),
    m_comment(start + p_offset)
{
}

CommentSegment::~CommentSegment()
{
}

std::string CommentSegment::printToStdOut() const
{
    std::stringstream return_value;
    return_value << "Comment Segment (offset=0x" << std::hex << m_offset
        << ", size=" << std::dec << m_size << ")\n"
        << "\tString=\"" << m_comment << "\"\n";
    return return_value.str();
}
