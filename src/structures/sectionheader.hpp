#ifndef SECTION_HEADER_HPP
#define SECTION_HEADER_HPP

#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>

namespace elf
{
    struct section_header_32
    {
        boost::uint32_t m_name;
        boost::uint32_t m_type;
        boost::uint32_t m_flags;
        boost::uint32_t m_addr;
        boost::uint32_t m_offset;
        boost::uint32_t m_size;
        boost::uint32_t m_link;
        boost::uint32_t m_info;
        boost::uint32_t m_addralign;
        boost::uint32_t m_entsize;
    };

    BOOST_STATIC_ASSERT(sizeof(section_header_32) == 40);

    struct section_header_64
    {
        boost::uint32_t m_name;
        boost::uint32_t m_type;
        boost::uint64_t m_flags;
        boost::uint64_t m_addr;
        boost::uint64_t m_offset;
        boost::uint64_t m_size;
        boost::uint32_t m_link;
        boost::uint32_t m_info;
        boost::uint64_t m_addralign;
        boost::uint64_t m_entsize;
    };

    BOOST_STATIC_ASSERT(sizeof(section_header_64) == 64);

    enum section_type
    {
        k_null = 0,
        k_progbits,
        k_symtab,
        k_strtab,
        k_rela,
        k_hash,
        k_dynamic,
        k_note,
        k_nobits,
        k_rel,
        k_shlib,
        k_dynsym,
        k_initArray = 14,
        k_finiArray = 15,
        k_gnuHash = 0x6ffffff6,
        k_gnuVersionR = 0x6ffffffe,
        k_gnuVersion = 0x6fffffff
    };

    enum section_flags
    {
        k_shwrite = 1,
        k_shalloc = 2,
        k_shexec = 4
    };
}

#endif
