#include "programheaders.hpp"
#include "abstract_segments.hpp"
#include "abstract_programheader.hpp"
#include "structures/programheader.hpp"

#include <boost/foreach.hpp>
#include <sstream>

ProgramHeaders::ProgramHeaders() :
    m_programHeaders()
{
}

ProgramHeaders::~ProgramHeaders()
{
}

void ProgramHeaders::setHeaders(const char* p_data, boost::uint16_t p_count,
                                boost::uint16_t p_size, bool p_is64, bool p_isLE)
{
    if (p_size == 0)
    {
        return;
    }

    for (std::size_t i = 0; i < p_count; ++i, p_data += p_size)
    {
        m_programHeaders.emplace_back(p_data, p_size, p_is64, p_isLE);
    }
}

void ProgramHeaders::extractSegments(AbstractSegments& p_segments)
{
    BOOST_FOREACH(const AbstractProgramHeader& header, m_programHeaders)
    {
        p_segments.makeSegmentFromProgramHeader(header);
    }
}

const std::vector<AbstractProgramHeader>& ProgramHeaders::getProgramHeaders() const
{
    return m_programHeaders;
}

std::string ProgramHeaders::printToStdOut() const
{
    std::stringstream returnValue;
    returnValue << "Program Header (count = " << m_programHeaders.size() << ")\n";
    BOOST_FOREACH(const AbstractProgramHeader& header, m_programHeaders)
    {
        returnValue << "\tEntry type=" << header.getName()
                    << " flags=" << std::dec << header.getFlags()
                    << " offset=0x" << std::hex << header.getOffset()
                    << " vaddr=0x" << header.getVirtualAddress()
                    << " paddr=0x" << header.getPhysicalAddress()
                    << " filesz=0x" << header.getFileSize()
                    << " memsz=0x" << header.getMemorySize()
                    << std::dec << "\n";
    }
    return returnValue.str();
}

void ProgramHeaders::evaluate(std::vector<std::pair<boost::int32_t, std::string> >& p_reasons) const
{
    bool found_load = false;
    std::size_t entry_count = 0;
    std::size_t load_count = 0;

    BOOST_FOREACH(const AbstractProgramHeader& header, m_programHeaders)
    {
        switch (header.getType())
        {
            case elf::k_pload:
                ++load_count;
                found_load = true;
                break;
            default:
                break;
        }
        ++entry_count;
    }

    if (load_count > 2)
    {
        p_reasons.push_back(std::make_pair(30, std::string("Found 2+ PT_LOAD. Possible post-compilation addition of code (cryptor or packer)")));
    }

    if (entry_count > 0)
    {
        if (!found_load)
        {
            p_reasons.push_back(std::make_pair(5, std::string("Didn't find PT_LOAD in the program headers")));
        }
    }
}

