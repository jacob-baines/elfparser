#include "abstract_symbol.hpp"

#include "structures/symtable_entry.hpp"

#include <boost/lexical_cast.hpp>
#include <sstream>

#ifdef __APPLE__
#include "endian.h"
#elif WINDOWS
#include "endian.h"
#else
#include <arpa/inet.h>
#endif

namespace
{
    std::string getSymBinding(boost::uint8_t p_info)
    {
        switch ((p_info >> 4) & 0x0f)
        {
            case elf::symbol::k_local:
                return "STB_LOCAL";
            case elf::symbol::k_global:
                return "STB_GLOBAL";
            case elf::symbol::k_weak:
                return "STB_WEAK";
            default:
                return boost::lexical_cast<std::string>((p_info >> 4) & 0x0f);
        }
    }

    std::string getSymType(boost::uint8_t p_info)
    {
        switch (p_info & 0x0f)
        {
            case elf::symbol::k_notype:
                return "STT_NOTYPE";
            case elf::symbol::k_object:
                return "STT_OBJECT";
            case elf::symbol::k_function:
                return "STT_FUNC";
            case elf::symbol::k_section:
                return "STT_SECTION";
            case elf::symbol::k_file:
                return "STT_FILE";
            case elf::symbol::k_common:
                return "STT_COMMON";
            case elf::symbol::k_tls:
                return "STT_TLS";
            default:
                return boost::lexical_cast<std::string>(p_info & 0x0f);
        }
    }
}

AbstractSymbol::AbstractSymbol(const char* p_data, boost::uint32_t p_offset,
                               bool p_is64, bool p_isLE) :
    m_symbol32(),
    m_symbol64(),
    m_name(),
    m_is64(p_is64),
    m_isLE(p_isLE)
{
    if (m_is64)
    {
        m_symbol64 = reinterpret_cast<const elf::symbol::symtable_entry64*>(p_data + p_offset);
    }
    else
    {
        m_symbol32 = reinterpret_cast<const elf::symbol::symtable_entry32*>(p_data + p_offset);
    }

    std::stringstream value;
    value << "0x" << std::hex << getValue();
    m_name.assign(value.str());
}

AbstractSymbol::AbstractSymbol(const AbstractSymbol& p_rhs) :
    m_symbol32(p_rhs.m_symbol32),
    m_symbol64(p_rhs.m_symbol64),
    m_name(p_rhs.m_name),
    m_is64(p_rhs.m_is64),
    m_isLE(p_rhs.m_isLE)
{
}

AbstractSymbol::~AbstractSymbol()
{
}

boost::uint32_t AbstractSymbol::getStructSize() const
{
    if (m_is64)
    {
        return sizeof(elf::symbol::symtable_entry64);
    }
    return sizeof(elf::symbol::symtable_entry32);
}

boost::uint8_t AbstractSymbol::getType() const
{
    if (m_is64)
    {
        return m_symbol64->m_info & 0x0f;
    }
    return m_symbol32->m_info & 0x0f;
}

boost::uint8_t AbstractSymbol::getInfo() const
{
    if (m_is64)
    {
        return m_symbol64->m_info;
    }
    return m_symbol32->m_info;
}

std::string AbstractSymbol::getTypeName() const
{
    return getSymType(getInfo());
}

std::string AbstractSymbol::getBinding() const
{
    return getSymBinding(getInfo());
}

boost::uint64_t AbstractSymbol::getValue() const
{
    if (m_is64)
    {
        return m_isLE ? m_symbol64->m_address : htobe64(m_symbol64->m_address);
    }
    return m_isLE ? m_symbol32->m_address : ntohl(m_symbol32->m_address);
}

boost::uint32_t AbstractSymbol::getNameIndex() const
{
    if (m_is64)
    {
        return m_isLE ? m_symbol64->m_name : ntohl(m_symbol64->m_name);
    }
    return m_isLE ? m_symbol32->m_name : ntohl(m_symbol32->m_name);
}

boost::uint16_t AbstractSymbol::getSectionIndex() const
{
    if (m_is64)
    {
        return m_isLE ? m_symbol64->m_shndx : ntohs(m_symbol64->m_shndx);
    }
    return m_isLE ? m_symbol32->m_shndx : ntohs(m_symbol32->m_shndx);
}

const std::string& AbstractSymbol::getName() const
{
    return m_name;
}

void AbstractSymbol::setName(const std::string& p_name)
{
    if (!p_name.empty())
    {
        m_name.assign(p_name);
    }
}
