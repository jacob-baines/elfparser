#ifndef NOTE_FORMAT_HPP
#define NOTE_FORMAT_HPP

namespace elf
{
    #pragma pack(push, 1)
    struct note
    {
        boost::uint32_t m_nameSize;
        boost::uint32_t m_descSize;
        boost::uint32_t m_type;
    };
    #pragma pack(pop)
}

#endif