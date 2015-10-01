#ifndef PROGRAM_HEADER_HPP
#define PROGRAM_HEADER_HPP

#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>

namespace elf
{
    struct program_header_32
    {
        boost::uint32_t m_type;
        boost::uint32_t m_offset;
        boost::uint32_t m_vaddr;
        boost::uint32_t m_paddr;
        boost::uint32_t m_filesz;
        boost::uint32_t m_memsz;
        boost::uint32_t m_flags;
        boost::uint32_t m_align;
    };

    BOOST_STATIC_ASSERT(sizeof(program_header_32) == 32);

    struct program_header_64
    {
        boost::uint32_t m_type;
        boost::uint32_t m_flags;
        boost::uint64_t m_offset;
        boost::uint64_t m_vaddr;
        boost::uint64_t m_paddr;
        boost::uint64_t m_filesz;
        boost::uint64_t m_memsz;
        boost::uint64_t m_align;
    };

    BOOST_STATIC_ASSERT(sizeof(program_header_64) == 56);

    enum programheader_type
    {
        k_pnull = 0,
        k_pload,
        k_pdynamic,
        k_pinterp,
        k_pnote,
        k_pshlib,
        k_pphdr,
        k_ptls,
        k_gnuEh = 0x6474e550,
        k_gnuStack = 0x6474e551,
        k_gnuRelRo = 0x6474e552,
        k_reginfo = 0x70000000,
        k_exidx = 0x70000001
    };

    enum programheader_flags
    {
        k_pfexec = 1,
        k_pfwrite = 2,
        k_pfread = 4
    };
}

#endif
