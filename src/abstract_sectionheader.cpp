#include "abstract_sectionheader.hpp"
#include "structures/sectionheader.hpp"
#include "structures/noteformat.hpp"
#include "abstract_segments.hpp"

#include <sstream>
#include <stdexcept>
#include <boost/foreach.hpp>

#ifdef __APPLE__
#include "endian.h"
#elif WINDOWS
#include "endian.h"
#else
#include <arpa/inet.h>
#endif

namespace
{
    std::string make_name(const char* p_start, boost::uint32_t p_offset,
                          boost::uint64_t p_size)
    {
        if (p_size < p_offset)
        {
            return std::string();
        }

        return std::string(p_start + p_offset);
    }
}

AbstractSectionHeader::AbstractSectionHeader(const char* p_data,
                                             boost::uint16_t p_size,
                                             const char* p_fileStart,
                                             boost::uint64_t p_fileSize,
                                             boost::uint8_t p_strIndex,
                                             const std::vector<AbstractSectionHeader>& p_sections,
                                             bool p_is64,
                                             bool p_isLE) :
    m_sections(p_sections),
    m_section_header32(),
    m_section_header64(),
    m_fileStart(p_fileStart),
    m_size(p_fileSize),
    m_strIndex(p_strIndex),
    m_is64(p_is64),
    m_isLE(p_isLE)
{
    if (p_is64)
    {
        if (p_size != sizeof(elf::section_header_64))
        {
            throw std::runtime_error("Unexpected section header size");
        }

        m_section_header64 = reinterpret_cast<const elf::section_header_64*>(p_data);
    }
    else
    {
        if (p_size != sizeof(elf::section_header_32))
        {
            throw std::runtime_error("Unexpected section header size.");
        }

        m_section_header32 = reinterpret_cast<const elf::section_header_32*>(p_data);
    }
}

AbstractSectionHeader::AbstractSectionHeader(const AbstractSectionHeader& p_rhs) :
    m_sections(p_rhs.m_sections),
    m_section_header32(p_rhs.m_section_header32),
    m_section_header64(p_rhs.m_section_header64),
    m_fileStart(p_rhs.m_fileStart),
    m_size(p_rhs.m_size),
    m_strIndex(p_rhs.m_strIndex),
    m_is64(p_rhs.m_is64),
    m_isLE(p_rhs.m_isLE)
{
}

AbstractSectionHeader::~AbstractSectionHeader()
{
}

bool AbstractSectionHeader::is64() const
{
    return m_is64;
}

bool AbstractSectionHeader::isLE() const
{
    return m_isLE;
}

bool AbstractSectionHeader::isExecutable() const
{
    if (getFlags() & elf::k_shexec)
    {
        return true;
    }
    return false;
}

bool AbstractSectionHeader::isWritable() const
{
    if (getFlags() & elf::k_shwrite)
    {
        return true;
    }
    return false;
}

std::string AbstractSectionHeader::getName() const
{
    boost::uint32_t offset = 0;
    if (m_is64)
    {
        offset = m_isLE ? m_section_header64->m_name : ntohl(m_section_header64->m_name);
    }
    else
    {
        offset = m_isLE ? m_section_header32->m_name : ntohl(m_section_header32->m_name);
    }
    if (m_sections.size() > m_strIndex)
    {
        offset += m_sections[m_strIndex].getPhysOffset();
    }
    else
    {
        return "Invalid String Index";
    }
    return make_name(m_fileStart, offset, m_size);
}

boost::uint32_t AbstractSectionHeader::getType() const
{
    if (m_is64)
    {
        return m_isLE ? m_section_header64->m_type : ntohl(m_section_header64->m_type);
    }
    return m_isLE ? m_section_header32->m_type : ntohl(m_section_header32->m_type);
}

std::string AbstractSectionHeader::getTypeString() const
{
    switch(getType())
    {
        case elf::k_null:
            return "K_NULL";
        case elf::k_progbits:
            return "K_PROGBITS";
        case elf::k_symtab:
            return "K_SYMTAB";
        case elf::k_strtab:
            return "K_STRTAB";
        case elf::k_rela:
            return "K_RELA";
        case elf::k_hash:
            return "K_HASH";
        case elf::k_dynamic:
            return "K_DYNAMIC";
        case elf::k_note:
            return "K_NOTE";
        case elf::k_nobits:
            return "K_NOBITS";
        case elf::k_rel:
            return "K_REL";
        case elf::k_shlib:
            return "K_SHLIB";
        case elf::k_dynsym:
            return "K_DYNSYM";
        case elf::k_initArray:
            return "K_INIT_ARRAY";
        case elf::k_finiArray:
            return "K_FINI_ARRAY";
        case elf::k_gnuHash:
            return "K_GNU_HASH";
        case elf::k_gnuVersionR:
            return "K_VERNEED";
        case elf::k_gnuVersion:
            return "K_VERSYM";
        default:
            {
                std::stringstream result;
                result << "0x" << std::hex << getType();
                return result.str();
            }
            break;
    }
    return "";
}

std::string AbstractSectionHeader::getFlagsString() const
{
    std::string result;
    if (getFlags() & elf::k_shwrite)
    {
        result.assign("Write");
    }
    if (getFlags() & elf::k_shexec)
    {
        if (result.empty())
        {
            result.assign("Execute");
        }
        else
        {
            result.append(", Execute");
        }
    }
    if (getFlags() & elf::k_shalloc)
    {
        if (result.empty())
        {
            result.assign("Alloc");
        }
        else
        {
            result.append(", Alloc");
        }
    }
    return result;
}

boost::uint64_t AbstractSectionHeader::getFlags() const
{
    if (m_is64)
    {
        return m_isLE ? m_section_header64->m_flags : htobe64(m_section_header64->m_flags);
    }
    return m_isLE ? m_section_header32->m_flags : ntohl(m_section_header32->m_flags);
}

boost::uint64_t AbstractSectionHeader::getVirtAddress() const
{
    if (m_is64)
    {
        return m_isLE ? m_section_header64->m_addr : htobe64(m_section_header64->m_addr);
    }
    return m_isLE ? m_section_header32->m_addr : ntohl(m_section_header32->m_addr);
}

std::string AbstractSectionHeader::getVirtAddressString() const
{
    std::stringstream result;
    result << "0x" << std::hex << getVirtAddress();
    return result.str();
}

boost::uint64_t AbstractSectionHeader::getPhysOffset() const
{
    if (m_is64)
    {
        return m_isLE ? m_section_header64->m_offset : htobe64(m_section_header64->m_offset);
    }
    return m_isLE ? m_section_header32->m_offset : ntohl(m_section_header32->m_offset);
}

boost::uint64_t AbstractSectionHeader::getSize() const
{
    if (m_is64)
    {
        return m_isLE ? m_section_header64->m_size : htobe64(m_section_header64->m_size);
    }
    return m_isLE ? m_section_header32->m_size : ntohl(m_section_header32->m_size);
}

boost::uint32_t AbstractSectionHeader::getLink() const
{
    if (m_is64)
    {
        return m_isLE ? m_section_header64->m_link : ntohl(m_section_header64->m_link);
    }
    return m_isLE ? m_section_header32->m_link : ntohl(m_section_header32->m_link);
}

boost::uint32_t AbstractSectionHeader::getInfo() const
{
    if (m_is64)
    {
        return m_isLE ? m_section_header64->m_info : ntohl(m_section_header64->m_info);
    }
    return m_isLE ? m_section_header32->m_info : ntohl(m_section_header32->m_info);
}

boost::uint64_t AbstractSectionHeader::getAddrAlign() const
{
    if (m_is64)
    {
        return m_isLE ? m_section_header64->m_addralign : htobe64(m_section_header64->m_addralign);
    }
    return m_isLE ? m_section_header32->m_addralign : ntohl(m_section_header32->m_addralign);
}

boost::uint64_t AbstractSectionHeader::getEntSize() const
{
    if (m_is64)
    {
        return m_isLE ? m_section_header64->m_entsize : htobe64(m_section_header64->m_entsize);
    }
    return m_isLE ? m_section_header32->m_entsize : ntohl(m_section_header32->m_entsize);
}
