#ifndef RELOCATION_HPP
#define RELOCATION_HPP

#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>

namespace elf
{
    struct relocation_32
    {
        boost::uint32_t m_offset;
        boost::uint32_t m_info;
    };

    BOOST_STATIC_ASSERT(sizeof(relocation_32) == 8);

    struct relocation_64
    {
        boost::uint64_t m_offset;
        boost::uint64_t m_info;
    };

    BOOST_STATIC_ASSERT(sizeof(relocation_64) == 16);

    #pragma pack(push, 1)
    struct rela_32
    {
        boost::uint32_t m_offset;
        boost::uint32_t m_info;
        boost::uint32_t m_addend;
    };

    BOOST_STATIC_ASSERT(sizeof(rela_32) == 12);

    struct rela_64
    {
        boost::uint64_t m_offset;
        boost::uint64_t m_info;
        boost::uint64_t m_addend;
    };
    #pragma pack(pop)
    BOOST_STATIC_ASSERT(sizeof(rela_64) == 24);
}

#endif
