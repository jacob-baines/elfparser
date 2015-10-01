#include "note_segment.hpp"
#include "../structures/noteformat.hpp"

#include <map>
#include <sstream>
#include <iostream>
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>

namespace
{
    const std::map<boost::uint32_t, std::string> type_map = boost::assign::map_list_of
        ( 1, "NT_GNU_ABI_TAG")
        ( 2, "NT_GNU_HWCAP")
        ( 3, "NT_GNU_BUILD_ID")
        ( 4, "NT_GNU_GOLD_VERSION");

    const std::map<boost::uint32_t, std::string> abi_map = boost::assign::map_list_of
        ( 0, "OS Linux")
        ( 1, "OS GNU")
        ( 2, "OS Solaris")
        ( 3, "OS FreeBSD");

    std::string parse_abi(const char* p_data, boost::uint32_t p_size)
    {
        const boost::uint32_t* abi = reinterpret_cast<const boost::uint32_t*>(p_data);

        if (p_size != 16)
        {
            return "";
        }

        std::string return_value;
        std::map<boost::uint32_t, std::string>::const_iterator abiIt = abi_map.find(abi[0]);
        if (abiIt != abi_map.end())
        {
            return_value.assign(abiIt->second);
        }
        else
        {
            return_value.assign(boost::lexical_cast<std::string>(abi[0]));
        }
        return_value.push_back(' ');
        return_value.append(boost::lexical_cast<std::string>(abi[1]));
        return_value.push_back('.');
        return_value.append(boost::lexical_cast<std::string>(abi[2]));
        return_value.push_back('.');
        return_value.append(boost::lexical_cast<std::string>(abi[3]));
        return return_value;
    }
}

NoteSegment::NoteSegment(const char* p_start, boost::uint32_t p_offset,
                         boost::uint32_t p_size, elf::section_type p_type) :
    SegmentType(p_start, p_offset, p_size, p_type),
    m_note(NULL),
    m_name(),
    m_noteType(),
    m_description()
{
    m_note = reinterpret_cast<const elf::note*>(p_start + p_offset);
    if (m_note->m_nameSize > 1 && m_note->m_nameSize < p_size)
    {
        m_name.assign(p_start + p_offset + sizeof(elf::note),
                      m_note->m_nameSize - 1);
    }

    std::map<boost::uint32_t, std::string>::const_iterator typeIt = type_map.find(m_note->m_type);
    if (typeIt != type_map.end())
    {
        m_noteType.assign(typeIt->second);
    }

    const char* desc_offset = p_start + p_offset + sizeof(elf::note) +
        m_note->m_nameSize;

    switch (m_note->m_type)
    {
        case 1:
            m_description = parse_abi(desc_offset, m_note->m_descSize);
            break;
        case 3:
            if (m_note->m_descSize < 256)
            {
                std::stringstream build_id;
                for (std::size_t i = 0; i < m_note->m_descSize; ++i)
                {
                    build_id << std::hex << (static_cast<unsigned int>(desc_offset[i]) & 0xff);
                }
                m_description.assign(build_id.str());
            }
            break;
        case 4:
            m_description.assign(desc_offset, m_note->m_descSize);
            break;
        default:
            break;
    }
}

NoteSegment::~NoteSegment()
{
}

std::string NoteSegment::printToStdOut() const
{
    std::stringstream return_value;
    return_value << "Note Segment (offset=0x" << std::hex << m_offset
                 << ", size=" << std::dec << m_size << ")\n";
    return_value << "\tName=\"" << m_name << "\"\n";
    return_value << "\tType=" << std::dec <<  m_note->m_type << "\n";
    if (!m_noteType.empty())
    {
        return_value << "\tType String=\"" << m_noteType << "\"\n";
    }
    return_value << "\tDescription=\"" << m_description << "\"\n";
    return return_value.str();
}
