#include "strtable_segment.hpp"

#include <cstring>
#include <sstream>
#include <boost/foreach.hpp>

#ifndef WINDOWS
#include <cxxabi.h>
#endif

StringTableSegment::StringTableSegment(const char* start,
                                       boost::uint32_t p_offset,
                                       boost::uint32_t p_size,
                                       elf::section_type p_type) :
    SegmentType(start, p_offset, p_size, p_type),
    m_stringsSet(),
    m_start(start + p_offset)
{
    const char* import_function = start + p_offset + 1;
    for (unsigned int i = 0; i < p_size; )
    {
        std::size_t length = strlen(import_function);
        if ((i + length) > p_size)
        {
            break;
        }
        std::string imported(import_function, length);
        m_stringsSet.insert(imported);
        i += length + 1;
        import_function += length + 1;
    }
}

StringTableSegment::~StringTableSegment()
{
}

std::size_t StringTableSegment::getSize() const
{
    return m_stringsSet.size();
}

std::string StringTableSegment::stringLookup(std::size_t p_index) const
{
#ifndef WINDOWS
    char* unmangled = NULL;
    size_t length = 0;
    int status = 0;
    unmangled  = abi::__cxa_demangle(m_start + p_index, unmangled,
                                        &length, &status);
    if (unmangled != NULL)
    {
        std::string fixed(unmangled);
        free(unmangled);
        return fixed;
    }
#endif
    return std::string(m_start + p_index);
}

std::string StringTableSegment::printToStdOut() const
{
    std::stringstream return_value;
    return_value << "String Table (offset=0x" << std::hex << m_offset
        << ", size=" << std::dec << m_size << ", entries="
        << m_stringsSet.size() << ")\n";

    BOOST_FOREACH(const std::string& p_ascii, m_stringsSet)
    {
        return_value << "\tString=\"" << p_ascii << "\"\n";
    }

    return return_value.str();
}
