#ifndef DYNAMICSTRUCT_HPP
#define DYNAMICSTRUCT_HPP

#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>
#include <sstream>
#include <string>

namespace elf
{
    namespace dynamic
    {
        struct dynamic_32
        {
            boost::uint32_t m_tag;
            union
            {
                boost::uint32_t m_val;
                boost::uint32_t m_ptr;
            };
        };

        BOOST_STATIC_ASSERT(8 == sizeof(dynamic_32));

        struct dynamic_64
        {
            boost::uint64_t m_tag;
            union
            {
                boost::uint64_t m_val;
                boost::uint64_t m_ptr;
            };
        };

        BOOST_STATIC_ASSERT(16 == sizeof(dynamic_64));

        enum tag
        {
            k_needed = 1,
            k_pltrelsz,
            k_pltgot,
            k_hash,
            k_strtab,
            k_symtab,
            k_rela,
            k_relasz,
            k_relaent,
            k_strsz,
            k_syment,
            k_init,
            k_fini,
            k_soname,
            k_rpath,
            k_symbolic,
            k_rel,
            k_relsz,
            k_relent,
            k_pltrel,
            k_debug,
            k_textrel,
            k_jmprel,
            k_bindnow,
            k_initarray,
            k_finiarray,
            k_init_arraysz,
            k_fini_arraysz,
            k_gnuhash = 0x6ffffef5
        };
    }
}

#endif
