#ifndef SYMTABLE_ENTRY_HPP
#define SYMTABLE_ENTRY_HPP

#include <string>
#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>

namespace elf
{
    namespace symbol
    {
        #pragma pack(push, 1)
        struct symtable_entry32
        {
            boost::uint32_t m_name;
            boost::uint32_t m_address;
            boost::uint32_t m_size;
            boost::uint8_t m_info;
            boost::uint8_t m_other;
            boost::uint16_t m_shndx;
        };

        BOOST_STATIC_ASSERT(sizeof(symtable_entry32) == 16);

        struct symtable_entry64
        {
            boost::uint32_t m_name;
            boost::uint8_t m_info;
            boost::uint8_t m_other;
            boost::uint16_t m_shndx;
            boost::uint64_t m_address;
            boost::uint64_t m_size;
        };

        BOOST_STATIC_ASSERT(sizeof(symtable_entry64) == 24);
        #pragma pack(pop)

        enum binding
        {
            k_local = 0,
            k_global,
            k_weak
        };

        enum type
        {
            k_notype = 0,
            k_object,
            k_function,
            k_section,
            k_file,
            k_common,
            k_tls
        };
    }
}

#endif
