#include "abstract_elfheader.hpp"
#include "structures/elfheader.hpp"
#include "structures/sectionheader.hpp"
#include "structures/programheader.hpp"

#include <sstream>
#include <cassert>
#include <cstring>
#include <stdexcept>
#ifdef __APPLE__
#include "endian.h"
#elif WINDOWS
#include "endian.h"
#else
#include <arpa/inet.h>
#endif

AbstractElfHeader::AbstractElfHeader() :
    m_is64(false),
    m_fileSize(0),
    m_header32(NULL),
    m_header64(NULL)
{
}

AbstractElfHeader::~AbstractElfHeader()
{
}

void AbstractElfHeader::setHeader(const char* p_data, std::size_t p_size)
{
    if (p_size < 45) // see size of true.asm from muppet labs
    {
        throw std::runtime_error("The file is too small to be an ELF binary");
    }

    m_fileSize = p_size;
    const elf::elf_header_32* header = reinterpret_cast<const elf::elf_header_32*>(p_data);
    if (header->m_magic0 != 0x7f || memcmp(header->m_ident + 1, "ELF", 3) != 0)
    {
        throw std::runtime_error("The file does not start with ELF magic.");
    }

    // we can't trust header->m_class. See muppet lab's
    if (header->m_phentsize == sizeof(elf::program_header_32) || header->m_phentsize == ntohs(sizeof(elf::program_header_32)))
    {
        m_is64 = false;
        m_header32 = header;
    }
    else
    {
        m_is64 = true;
        m_header64 = reinterpret_cast<const elf::elf_header_64*>(p_data);
        if (m_header64->m_phentsize != sizeof(elf::program_header_64) && m_header64->m_phentsize != ntohs(sizeof(elf::program_header_64)))
        {
            throw std::runtime_error("Unable to determine 32 bit vs. 64 bit");
        }
    }
}

bool AbstractElfHeader::is64() const
{
    assert(m_header32 != NULL || m_header64 != NULL);
    return m_is64;
}

std::string AbstractElfHeader::getMagic() const
{
    std::stringstream result;
    result << std::hex << static_cast<int>((m_is64 ? m_header64->m_magic0 : m_header32->m_magic0)) << " ";
    result << std::hex << static_cast<int>((m_is64 ? m_header64->m_magic1 : m_header32->m_magic1)) << " ";
    result << std::hex << static_cast<int>((m_is64 ? m_header64->m_magic2 : m_header32->m_magic2)) << " ";
    result << std::hex << static_cast<int>((m_is64 ? m_header64->m_magic3 : m_header32->m_magic3));
    return result.str();
}

std::string AbstractElfHeader::getType() const
{
    boost::uint16_t type = 0;
    if (m_is64)
    {
        type = isLE() ? m_header64->m_type : ntohs(m_header64->m_type);
    }
    else
    {
        type = isLE() ? m_header32->m_type : ntohs(m_header32->m_type);
    }

    switch (type)
    {
        case elf::k_etnone:
            return "ET_NONE";
        case elf::k_etrel:
            return "ET_REL";
        case elf::k_etexec:
            return "ET_EXEC";
        case elf::k_etdyn:
            return "ET_DYN";
        case elf::k_etcore:
            return "ET_CORE";
        default:
            return "UNKNOWN";
    }
}

std::string AbstractElfHeader::getOSABI() const
{
    boost::uint8_t os = m_is64 ? m_header64->m_os : m_header32->m_os;
    switch (os)
    {
        case 0:
            return "System V";
        case 1:
            return "HP-UX";
        case 2:
            return "NetBSD";
        case 3:
            return "Linux";
        case 6:
            return "Solaris";
        case 7:
            return "AIX";
        case 8:
            return "IRIX";
        case 9:
            return "FreeBSD";
        case 0x0c:
            return "OpenBSD";
        default:
        {
            std::stringstream result;
            result << "0x" << std::hex << static_cast<int>(os);
            return result.str();
        }
    }
}

boost::uint64_t AbstractElfHeader::getEntryPoint() const
{
    assert(m_header32 != NULL || m_header64 != NULL);
    if (m_is64)
    {
        return isLE() ? m_header64->m_entry : htobe64(m_header64->m_entry);
    }
    return isLE() ? m_header32->m_entry : ntohl(m_header32->m_entry);
}

std::string AbstractElfHeader::getEntryPointString() const
{
    std::stringstream result;
    result << "0x" << std::hex << getEntryPoint();
    return result.str();
}

std::string AbstractElfHeader::getVersion() const
{
    std::stringstream result;
    if (m_is64)
    {
        result << (isLE() ? m_header64->m_version : ntohl(m_header64->m_version));
    }
    else
    {
        result << (isLE() ? m_header32->m_version : ntohl(m_header32->m_version));
    }
    return result.str();
}

boost::uint32_t AbstractElfHeader::getProgramOffset() const
{
    assert(m_header32 != NULL || m_header64 != NULL);
    if (m_is64)
    {
        return isLE() ? m_header64->m_phoff : htobe64(m_header64->m_phoff);
    }
    return isLE() ? m_header32->m_phoff : ntohl(m_header32->m_phoff);
}

boost::uint16_t AbstractElfHeader::getProgramCount() const
{
    assert(m_header32 != NULL || m_header64 != NULL);
    if (m_is64)
    {
        return isLE() ? m_header64->m_phnum : ntohs(m_header64->m_phnum);
    }
    return isLE() ? m_header32->m_phnum : ntohs(m_header32->m_phnum);
}

boost::uint16_t AbstractElfHeader::getProgramSize() const
{
    assert(m_header32 != NULL || m_header64 != NULL);
    if (m_is64)
    {
        return isLE() ? m_header64->m_phentsize : ntohs(m_header64->m_phentsize);
    }
    return isLE() ? m_header32->m_phentsize : ntohs(m_header32->m_phentsize);
}

boost::uint32_t AbstractElfHeader::getSectionOffset() const
{
    assert(m_header32 != NULL || m_header64 != NULL);
    if (m_is64)
    {
        return isLE() ? m_header64->m_shoff : htobe64(m_header64->m_shoff);
    }
    return isLE() ? m_header32->m_shoff : ntohl(m_header32->m_shoff);
}

boost::uint16_t AbstractElfHeader::getSectionSize() const
{
    assert(m_header32 != NULL || m_header64 != NULL);

    if (m_is64)
    {
        return isLE() ? m_header64->m_shentsize : ntohs(m_header64->m_shentsize);
    }
    return isLE() ? m_header32->m_shentsize : ntohs(m_header32->m_shentsize);
}

boost::uint16_t AbstractElfHeader::getSectionCount() const
{
    assert(m_header32 != NULL || m_header64 != NULL);
    if (m_is64)
    {
        return isLE() ? m_header64->m_shnum : ntohs(m_header64->m_shnum);
    }
    return isLE() ? m_header32->m_shnum : ntohs(m_header32->m_shnum);
}

boost::uint16_t AbstractElfHeader::getStringTableIndex() const
{
    assert(m_header32 != NULL || m_header64 != NULL);
    if (m_is64)
    {
        return isLE() ? m_header64->m_shtrndx : ntohs(m_header64->m_shtrndx);
    }
    return isLE() ? m_header32->m_shtrndx : ntohs(m_header32->m_shtrndx);
}

boost::uint32_t AbstractElfHeader::getStringTableOffset(const char* p_start) const
{
    if (getStringTableIndex() == 0)
    {
        return 0;
    }

    if (m_is64)
    {
        boost::uint32_t offset = getSectionOffset() + (getStringTableIndex() * sizeof(elf::section_header_64));
        const elf::section_header_64* strTab =
            reinterpret_cast<const elf::section_header_64*>(p_start + offset);
        return strTab->m_offset;
    }
    else
    {
        boost::uint32_t offset = getSectionOffset() + (getStringTableIndex() * sizeof(elf::section_header_32));
        const elf::section_header_32* strTab =
            reinterpret_cast<const elf::section_header_32*>(p_start + offset);
        return strTab->m_offset;
    }
}

bool AbstractElfHeader::isLE() const
{
    // this is hack to get around abuse of the encoding field
    assert(m_header32 != NULL || m_header64 != NULL);
    if (m_is64)
    {
        return (m_header64->m_phentsize == sizeof(elf::program_header_64) ? true : false);
    }

    return (m_header32->m_phentsize == sizeof(elf::program_header_32) ? true : false);
}

std::string AbstractElfHeader::getMachine() const
{
    boost::uint16_t machine = m_is64 ? m_header64->m_machine : m_header32->m_machine;
    if (!isLE())
    {
        machine = ntohs(machine);
    }

    switch (machine)
    {
        case elf::k_em386:
            return "x86";
        case elf::k_emARM:
            return "ARM";
        case elf::k_emMIPS:
            return "MIPS";
        case elf::k_emPPC:
            return "PowerPC";
        case elf::k_emx8664:
            return "x86_64";
        case elf::k_emnone:
            return "None";
        default:
            return "Unknown";
    }
}

std::string AbstractElfHeader::getEncoding() const
{
    boost::uint8_t encoding = m_header64 != NULL ? m_header64->m_encoding : m_header32->m_encoding;
    switch (encoding)
    {
        case 0:
            return "Invalid";
        case 1:
            return "Little Endian";
        case 2:
            return "Big Endian";
        default:
            return "Unknown";
    }
}

std::string AbstractElfHeader::getABIVersion() const
{
    std::stringstream result;
    if (m_is64)
    {
        result << static_cast<int>(m_header64->m_abi);
    }
    else
    {
        result << static_cast<int>(m_header32->m_abi);
    }
    return result.str();
}

std::string AbstractElfHeader::getClass() const
{
    if (m_is64)
    {
        return "64-bit";
    }
    else
    {
        return "32-bit";
    }
}

std::string AbstractElfHeader::getFileVersion() const
{
    std::stringstream result;
    result << static_cast<int>(m_is64 ? m_header64->m_fileversion : m_header32->m_fileversion);
    return result.str();
}

std::string AbstractElfHeader::getEHSize() const
{
    assert(m_header32 != NULL || m_header64 != NULL);
    boost::uint16_t value = 0;
    if (m_is64)
    {
        value = isLE() ? m_header64->m_ehsize : ntohs(m_header64->m_ehsize);
    }
    else
    {
        value = isLE() ? m_header32->m_ehsize : ntohs(m_header32->m_ehsize);
    }
    std::stringstream result;
    result << value;
    return result.str();
}

std::string AbstractElfHeader::getFlags() const
{
    assert(m_header32 != NULL || m_header64 != NULL);
    boost::uint32_t value = 0;
    if (m_is64)
    {
        value = isLE() ? m_header64->m_flags : ntohl(m_header64->m_flags);
    }
    else
    {
        value = isLE() ? m_header32->m_flags : ntohl(m_header32->m_flags);
    }
    std::stringstream result;
    result << "0x" << std::hex << value;
    return result.str();
}

std::string AbstractElfHeader::printToStdOut() const
{
    std::stringstream return_value;
    return_value << "ELF Header\n\tclass=";
    return_value << getClass() << "\n";
    return_value << "\tencoding=" << getEncoding() << "\n";
    return_value << "\tfileversion=" << getFileVersion() << "\n";
    return_value << "\tos=" << getOSABI() << "\n";
    return_value << "\tabi=" << getABIVersion() << "\n";
    return_value << "\ttype=" << getType() << "\n";
    return_value << "\tmachine=" << getMachine() << "\n";
    return_value << "\tversion=" << getVersion() << "\n";
    return_value << "\tentryPoint=" << getEntryPointString() << "\n";
    return_value << "\tphoffset=" << "0x" << std::hex << getProgramOffset() << "\n";
    return_value << "\tshoffset=" << "0x" << std::hex << getSectionOffset() << "\n";
    return_value << "\tflags=" << getFlags() << "\n";
    return_value << "\tehsize=" << getEHSize() << "\n";
    return_value << "\tphentsize=" << getProgramSize() << "\n";
    return_value << "\tphnum=" << getProgramCount() << "\n";
    return_value << "\tshentsize=" << getSectionSize() << "\n";
    return_value << "\tshnum=" << getSectionCount() << "\n";
    return_value << "\tshtrndx=" << getStringTableIndex() << "\n";
    return return_value.str();
}

void AbstractElfHeader::evaluate(std::vector<std::pair<boost::int32_t, std::string> >&,
                                 std::map<elf::Capabilties, std::set<std::string> >& p_capabilities) const
{
    if (getProgramOffset() != 0 && getProgramOffset() < 45)
    {
        p_capabilities[elf::k_antidebug].insert("Possible compact ELF: program header overlaps with ELF header");
    }
    if (getEncoding() == "Invalid")
    {
        p_capabilities[elf::k_antidebug].insert("Possible compact ELF: invalid encoding in ELF header");
    }
    if (getSectionOffset() != 0 && getSectionCount() == 0)
    {
        p_capabilities[elf::k_antidebug].insert("Possible compact ELF: section count 0 but section offset non-zero");
    }
    if (m_header32 != NULL)
    {
        if (m_header32->m_class != 1 && m_header32->m_class != 2)
        {
            p_capabilities[elf::k_antidebug].insert("Possible compact ELF: invalid class in ELF header");
        }
    }
    else
    {
        if (m_header64->m_class != 1 && m_header64->m_class != 2)
        {
            p_capabilities[elf::k_antidebug].insert("Possible compact ELF: invalid class in ELF header");
        }
    }
}
