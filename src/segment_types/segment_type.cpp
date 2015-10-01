#include "segment_type.hpp"

SegmentType::SegmentType (const char*, boost::uint32_t p_offset,
                  boost::uint32_t p_size, elf::section_type p_type) :
    m_type(p_type),
    m_offset(p_offset),
    m_size(p_size),
    m_strings(NULL)
{
}

SegmentType::~SegmentType()
{
}

elf::section_type SegmentType::getType() const
{
    return m_type;
}

boost::uint32_t SegmentType::getOffset() const
{
    return m_offset;
}

void SegmentType::setStrSegment(SegmentType* p_strSegment)
{
    m_strings = p_strSegment;
}
