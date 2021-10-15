#include "abstract_dynamic.hpp"
#include "structures/dynamicstruct.hpp"

AbstractDynamicEntry::AbstractDynamicEntry(boost::uint64_t p_tag, boost::uint64_t p_value)
{
    m_tag = p_tag;
    m_value = p_value;
}

AbstractDynamicEntry::AbstractDynamicEntry(const AbstractDynamicEntry &p_rhs)
{
    m_tag = p_rhs.m_tag;
    m_value = p_rhs.m_value;
    m_stringValue = p_rhs.m_stringValue;
}

AbstractDynamicEntry::~AbstractDynamicEntry()
{  }

bool AbstractDynamicEntry::validString() const
{
    return !m_stringValue.empty();
}

void AbstractDynamicEntry::createString(const char *p_strOffset)
{
    m_stringValue.assign(createValue(m_tag, m_value, p_strOffset));
}

std::string AbstractDynamicEntry::createTag(boost::uint64_t p_tag) const
{
    switch (p_tag)
    {
        case elf::dynamic::k_needed:
            return "NEEDED";
        case elf::dynamic::k_pltrelsz:
            return "PLTRELSZ";
        case elf::dynamic::k_pltgot:
            return "PLTGOT";
        case elf::dynamic::k_hash:
            return "HASH";
        case elf::dynamic::k_strtab:
            return "STRTAB";
        case elf::dynamic::k_symtab:
            return "SYMTAB";
        case elf::dynamic::k_rela:
            return "RELA";
        case elf::dynamic::k_relasz:
            return "RELASZ";
        case elf::dynamic::k_relaent:
            return "RELAENT";
        case elf::dynamic::k_strsz:
            return "STRSZ";
        case elf::dynamic::k_syment:
            return "SYMENT";
        case elf::dynamic::k_init:
            return "INIT";
        case elf::dynamic::k_fini:
            return "FINI";
        case elf::dynamic::k_soname:
            return "SONAME";
        case elf::dynamic::k_rpath:
            return "RPATH";
        case elf::dynamic::k_symbolic:
            return "SYMBOLIC";
        case elf::dynamic::k_rel:
            return "REL";
        case elf::dynamic::k_relsz:
            return "RELSZ";
        case elf::dynamic::k_relent:
            return "RELENT";
        case elf::dynamic::k_pltrel:
            return "PLTREL";
        case elf::dynamic::k_debug:
            return "DEBUG";
        case elf::dynamic::k_textrel:
            return "TEXTREL";
        case elf::dynamic::k_jmprel:
            return "JMPREL";
        case elf::dynamic::k_bindnow:
            return "BIND_NOW";
        case elf::dynamic::k_initarray:
            return "INIT_ARRAY";
        case elf::dynamic::k_finiarray:
            return "FINI_ARRAY";
        case elf::dynamic::k_init_arraysz:
            return "INIT_ARRAYSZ";
        case elf::dynamic::k_fini_arraysz:
            return "FINI_ARRAYSZ";
        case elf::dynamic::k_gnuhash:
            return "GNU_HASH";
    default:
        std::stringstream valueString;
        valueString << std::hex << "0x" << p_tag << std::dec;
        return valueString.str();
    }
    return CEXIT_SUCCESS;
}

std::string AbstractDynamicEntry::createValue(boost::uint64_t p_tag, boost::uint64_t p_value, const char *p_strTab) const
{
    switch (p_tag)
    {
    case elf::dynamic::k_needed:
    case elf::dynamic::k_soname:
    case elf::dynamic::k_rpath:
        if (p_strTab != NULL)
            return p_strTab + p_value;
    }
    return CEXIT_SUCCESS;
}

boost::uint64_t AbstractDynamicEntry::getTag() const
{
    return m_tag;
}

boost::uint64_t AbstractDynamicEntry::getValue() const
{
    return m_value;
}

const std::string &AbstractDynamicEntry::getString() const
{
    return m_stringValue;
}

std::string AbstractDynamicEntry::printToStdOut() const 
{
    std::stringstream valueString;
    valueString << "\t tag = " << createTag(m_tag) << ", value = ";
    if (validString())
        valueString << m_stringValue;
    else
        valueString << "0x" << std::hex << m_value;
    valueString << "\n";

    return valueString.str();
}