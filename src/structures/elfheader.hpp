#ifndef ELF_HEADER_HPP
#define ELF_HEADER_HPP

#include <string>
#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>

namespace elf
{
    enum elf_class
    {
        k_32 = 1,
        k_64 = 2
    };

    enum elf_encoding
    {
        k_littleEndian = 1,
        k_bigEndian = 2
    };

    enum elf_type
    {
        k_etnone = 0,
        k_etrel,
        k_etexec,
        k_etdyn,
        k_etcore
    };

    enum elf_machine
    {
        k_emnone = 0,
        k_em386 = 3,
        k_emMIPS = 8,
        k_emPPC = 20,
        k_emARM = 40,
        k_emx8664 = 62
    };

    #pragma pack(push, 1)
    struct elf_header_32
    {
        union
        {
            boost::uint8_t m_ident[16];
            struct
            {
                boost::uint8_t m_magic0;
                boost::uint8_t m_magic1;
                boost::uint8_t m_magic2;
                boost::uint8_t m_magic3;
                boost::uint8_t m_class;
                boost::uint8_t m_encoding;
                boost::uint8_t m_fileversion;
                boost::uint8_t m_os;
                boost::uint8_t m_abi;
            };
        };
        boost::uint16_t m_type;
        boost::uint16_t m_machine;
        boost::uint32_t m_version;
        boost::uint32_t m_entry;
        boost::uint32_t m_phoff;
        boost::uint32_t m_shoff;
        boost::uint32_t m_flags;
        boost::uint16_t m_ehsize;
        boost::uint16_t m_phentsize;
        boost::uint16_t m_phnum;
        boost::uint16_t m_shentsize;
        boost::uint16_t m_shnum;
        boost::uint16_t m_shtrndx;
    };

    BOOST_STATIC_ASSERT(sizeof(elf_header_32) == 52);

    struct elf_header_64
    {
        union
        {
            boost::uint8_t m_ident[16];
            struct
            {
                boost::uint8_t m_magic0;
                boost::uint8_t m_magic1;
                boost::uint8_t m_magic2;
                boost::uint8_t m_magic3;
                boost::uint8_t m_class;
                boost::uint8_t m_encoding;
                boost::uint8_t m_fileversion;
                boost::uint8_t m_os;
                boost::uint8_t m_abi;
            };
        };
        boost::uint16_t m_type;
        boost::uint16_t m_machine;
        boost::uint32_t m_version;
        boost::uint64_t m_entry;
        boost::uint64_t m_phoff;
        boost::uint64_t m_shoff;
        boost::uint32_t m_flags;
        boost::uint16_t m_ehsize;
        boost::uint16_t m_phentsize;
        boost::uint16_t m_phnum;
        boost::uint16_t m_shentsize;
        boost::uint16_t m_shnum;
        boost::uint16_t m_shtrndx;
    };

    BOOST_STATIC_ASSERT(sizeof(elf_header_64) == 64);
    #pragma pack(pop)
}

#endif