#include "abstract_segments.hpp"

#include "segment.hpp"
#include "structures/programheader.hpp"
#include "structures/sectionheader.hpp"
#include "structures/symtable_entry.hpp"
#include "abstract_programheader.hpp"
#include "abstract_sectionheader.hpp"

#include "segment_types/comment_segment.hpp"
#include "segment_types/debuglink_segment.hpp"
#include "segment_types/interp_segment.hpp"
#include "segment_types/note_segment.hpp"
#include "segment_types/readonly_segment.hpp"
#include "segment_types/strtable_segment.hpp"

#include <sstream>
#include <iostream>
#include <boost/foreach.hpp>

AbstractSegments::AbstractSegments() :
    m_data(NULL),
    m_size(0),
    m_sections(),
    m_programs(),
    m_types(),
    m_offsets(),
    m_baseAddress(0),
    m_dynamic(),
    m_dynSymbols(),
    m_otherSymbols(),
    m_initArray("InitArray"),
    m_ctorsArray("CtorsArray"),
    m_setBase(false),
    m_is64(false),
    m_isLE(false),
    m_isDY(false),
    m_fakeDynamicStringTable(false)
{
}

AbstractSegments::~AbstractSegments()
{
}

std::set<std::string> AbstractSegments::getFiles() const
{
    std::set<std::string> combined = m_dynSymbols.getFiles();
    BOOST_FOREACH(const Symbols& symbol, m_otherSymbols)
    {
        const std::set<std::string>& unique(symbol.getFiles());
        BOOST_FOREACH(const std::string& value, unique)
        {
            combined.insert(value);
        }
    }
    return combined;
}

void AbstractSegments::setStart(const char* p_data, boost::uint32_t p_size,
                                bool p_is64, bool p_isLE, bool p_isDY)
{
    m_data = p_data;
    m_size = p_size;
    m_is64 = p_is64;
    m_isLE = p_isLE;
    m_isDY = p_isDY;
}

void AbstractSegments::makeSegmentFromSectionHeader(const AbstractSectionHeader& p_header)
{
    m_sections.emplace_back(p_header.getName(), p_header.getTypeString(), p_header.getPhysOffset(),
                            p_header.getVirtAddress(), p_header.getSize(),
                            p_header.getLink(), p_header.isExecutable(), p_header.isWritable(),
                            p_header.getType() == elf::k_dynamic);

}

void AbstractSegments::makeSegmentFromProgramHeader(const AbstractProgramHeader& p_header)
{
    if (!m_setBase && p_header.getType() == elf::k_pload)
    {
        m_baseAddress = p_header.getVirtualAddress();
        m_setBase = true;
    }
    m_programs.emplace_back(p_header.getName(), p_header.getName(), p_header.getOffset(),
                            p_header.getVirtualAddress(),
                            p_header.getMemorySize() ? p_header.getMemorySize() : p_header.getFileSize(),
                            0, p_header.isExecutable(), p_header.isWritable(),
                            p_header.getType() == elf::k_pdynamic);
}

void AbstractSegments::createDynamic()
{
    BOOST_FOREACH(const Segment& program, m_programs)
    {
        if (program.isDynamic())
        {
            m_dynamic.createDynamic(m_data, program.getPhysOffset(),
                                    program.getSize(), m_baseAddress,
                                    m_is64, m_isLE, *this);

            // build the symtab based off of the dynamic section
            boost::uint64_t symTab = getOffsetFromVirt(m_dynamic.getSymbolTableVirtAddress());
            boost::uint64_t strTab = getOffsetFromVirt(m_dynamic.getStringTableVirtualAddress());
            m_dynSymbols.createSymbols(m_data, m_size, symTab, m_dynamic.getSymbolTableSize(),
                                       strTab, m_dynamic.getStringTableSize(),
                                       *this, m_is64, m_isLE, m_isDY);

            // pull out the init array
            if (m_dynamic.getInitArray() != 0 && m_dynamic.getInitArrayEntries() != 0)
            {
                m_initArray.set(m_data, m_size, getOffsetFromVirt(m_dynamic.getInitArray()),
                                m_dynamic.getInitArrayEntries(), m_is64, m_isLE);
                m_offsets.insert(m_data + getOffsetFromVirt(m_dynamic.getInitArray()));
            }

            // make sure we don't parse these again
            m_offsets.insert(m_data + symTab);
            m_offsets.insert(m_data + program.getPhysOffset());
            return;
        }
    }

    BOOST_FOREACH(const Segment& section, m_sections)
    {
        if (section.isDynamic())
        {
            m_dynamic.createDynamic(m_data, section.getPhysOffset(),
                                    section.getSize(), m_baseAddress,
                                    m_is64, m_isLE, *this);

            // build the symtab based off of the dynamic section
            boost::uint64_t symTab = getOffsetFromVirt(m_dynamic.getSymbolTableVirtAddress());
            boost::uint64_t strTab = getOffsetFromVirt(m_dynamic.getStringTableVirtualAddress());
            m_dynSymbols.createSymbols(m_data, m_size, symTab, m_dynamic.getSymbolTableSize(),
                                       strTab, m_dynamic.getStringTableSize(),
                                       *this, m_is64, m_isLE, m_isDY);

            // make sure we don't parse these again
            m_offsets.insert(m_data + symTab);
            m_offsets.insert(m_data + section.getPhysOffset());
            return;
        }
    }
}

void AbstractSegments::generateSegments()
{
    // Track the string and symbol tables are correlate them using link after
    // the first pass through.
    std::size_t tableIndex = 0;
    std::set<std::size_t> strTab;
    std::set<std::size_t> symTab;
    BOOST_FOREACH(const Segment& section, m_sections)
    {
        if (m_offsets.find(m_data + section.getPhysOffset()) == m_offsets.end())
        {
            if (section.getType() == "K_NOTE")
            {
                m_types.push_back(new NoteSegment(m_data, section.getPhysOffset(), section.getSize(), elf::k_note));
                m_offsets.insert(m_data + section.getPhysOffset());
            }
            else if (section.getType() == "K_PROGBITS")
            {
                if (section.getName() == ".comment")
                {
                    m_types.push_back(new CommentSegment(m_data, section.getPhysOffset(), section.getSize(), elf::k_progbits));
                    m_offsets.insert(m_data + section.getPhysOffset());
                }
                else if (section.getName() == ".gnu_debuglink")
                {
                    m_types.push_back(new DebugLinkSegment(m_data, section.getPhysOffset(), section.getSize(), elf::k_progbits));
                    m_offsets.insert(m_data + section.getPhysOffset());
                }
                else if (section.getName() == ".interp")
                {
                    m_types.push_back(new InterpSegment(m_data, section.getPhysOffset(), section.getSize(), elf::k_progbits));
                    m_offsets.insert(m_data + section.getPhysOffset());
                }
                else if (section.getName() == ".rodata")
                {
                    m_types.push_back(new ReadOnlySegment(m_data, section.getPhysOffset(), section.getSize(), elf::k_progbits));
                    m_offsets.insert(m_data + section.getPhysOffset());
                }
                else if (m_ctorsArray.getOffset() == 0 && section.getName() == ".ctors")
                {
                    m_ctorsArray.set(m_data, m_size, section.getPhysOffset(),
                                     section.getSize() / (m_is64 ? 8 : 4), m_is64, m_isLE);
                    m_offsets.insert(m_data + section.getPhysOffset());
                }
            }
            else if (section.getType() == "K_STRTAB")
            {
                strTab.insert(tableIndex);
            }
            else if (section.getType() == "K_SYMTAB")
            {
                symTab.insert(tableIndex);
            }
            else if (m_initArray.getOffset() == 0 && section.getType() == "K_INIT_ARRAY")
            {
                m_initArray.set(m_data, m_size, section.getPhysOffset(),
                                section.getSize() / (m_is64 ? 8 : 4), m_is64, m_isLE);
                m_offsets.insert(m_data + section.getPhysOffset());
            }
            else if (section.getType() == "K_DYNAMIC")
            {
                assert("should not hit here" == 0);
            }
        }
        ++tableIndex;
    }

    // loop over the symbol tables we saved and resolve the links.
    BOOST_FOREACH(std::size_t index, symTab)
    {
        // validate that the symtab has a good strtab
        if (m_sections.size() > m_sections[index].getLink() &&
            m_sections[m_sections[index].getLink()].getType() == "K_STRTAB")
        {
            // create the strtab segment
            std::size_t link = m_sections[index].getLink();
            strTab.erase(link);

            // check to see if this is a fake/copied symbol table.
            if (m_sections[link].getPhysOffset() != 0 &&
                m_sections[link].getVirtAddress() != 0 &&
                m_sections[link].getVirtAddress() == m_dynamic.getStringTableVirtualAddress() &&
                m_sections[link].getPhysOffset() != getOffsetFromVirt(m_dynamic.getStringTableVirtualAddress()))
            {
                m_fakeDynamicStringTable = true;
            }
            else
            {
                m_types.push_back(new StringTableSegment(m_data, 
                    m_sections[link].getPhysOffset(), m_sections[link].getSize(), elf::k_strtab));
                m_offsets.insert(m_data + m_sections[link].getPhysOffset());
            }

            // create the symtab segment
            Symbols* otherSymbols = new Symbols();
            otherSymbols->createSymbols(m_data, m_size, m_sections[index].getPhysOffset(),
                                        m_sections[index].getSize(),
                                        m_sections[link].getPhysOffset(),
                                        m_sections[link].getSize(),
                                        *this, m_is64, m_isLE, m_isDY);
            m_otherSymbols.push_back(otherSymbols);
            m_offsets.insert(m_data + m_sections[index].getPhysOffset());
        }
    }

    // for any remaining strtab just create the segment
    BOOST_FOREACH(std::size_t index, strTab)
    {
        m_types.push_back(new StringTableSegment(m_data, m_sections[index].getPhysOffset(),
            m_sections[index].getSize(), elf::k_strtab));
        m_offsets.insert(m_data + m_sections[index].getPhysOffset());
    }

    // segments are done try to resolve init array functions
    std::vector<std::pair<boost::uint64_t, std::string> >& initArray = m_initArray.getEntries();
    for (std::size_t j = 0 ; j < initArray.size(); ++j)
    {
        for (std::size_t i = 0; i < m_otherSymbols.size(); ++i)
        {
            std::string resolved = m_otherSymbols[i].findSymbol(initArray[j].first);
            if (!resolved.empty())
            {
                initArray[j].second.assign(resolved);
            }
        }
    }
    std::vector<std::pair<boost::uint64_t, std::string> >& ctorsArray = m_ctorsArray.getEntries();
    for (std::size_t j = 0 ; j < ctorsArray.size(); ++j)
    {
        for (std::size_t i = 0; i < m_otherSymbols.size(); ++i)
        {
            std::string resolved = m_otherSymbols[i].findSymbol(ctorsArray[j].first);
            if (!resolved.empty())
            {
                ctorsArray[j].second.assign(resolved);
            }
        }
    }
}

boost::uint64_t AbstractSegments::getBaseAddress() const
{
    return m_baseAddress;
}

boost::uint64_t AbstractSegments::getOffsetFromVirt(boost::uint64_t p_virtual) const
{
    BOOST_FOREACH(const Segment& program, m_programs)
    {
        if (program.getVirtAddress() <= p_virtual &&
            (program.getVirtAddress() + program.getSize()) > p_virtual)
        {
            return program.getPhysOffset() + (p_virtual - program.getVirtAddress());
        }
    }
    BOOST_FOREACH(const Segment& section, m_sections)
    {
        if (section.getVirtAddress() <= p_virtual &&
            (section.getVirtAddress() + section.getSize()) > p_virtual)
        {
            return section.getPhysOffset() + (p_virtual - section.getVirtAddress());
        }
    }
    return 0;
}

void AbstractSegments::evaluate(std::vector<std::pair<boost::int32_t, std::string> >& p_reasons,
                                std::map<elf::Capabilties, std::set<std::string> >& p_capabilities) const
{
    m_dynamic.evaluate(p_reasons, p_capabilities);
    m_dynSymbols.evaluate(p_reasons, p_capabilities);

    BOOST_FOREACH(const SegmentType& seg, m_types)
    {
        seg.evaluate(p_reasons, p_capabilities);
    }

    BOOST_FOREACH(const Symbols& sym, m_otherSymbols)
    {
        sym.evaluate(p_reasons, p_capabilities);
    }

    if (m_fakeDynamicStringTable)
    {
        p_capabilities[elf::k_antidebug].insert("Fake symbol table strings in sections");
    }
}

std::vector<AbstractSymbol> AbstractSegments::getAllSymbols() const
{
    std::vector<AbstractSymbol> symbols(m_dynSymbols.getSymbols());
    BOOST_FOREACH(const Symbols& other, m_otherSymbols)
    {
        const std::vector<AbstractSymbol>& others(other.getSymbols());
        BOOST_FOREACH(const AbstractSymbol& newSymbols, others)
        {
            symbols.push_back(newSymbols);
        }
    }

    return symbols;
}

const DynamicSection& AbstractSegments::getDynamicSection() const
{
    return m_dynamic;
}

const Symbols& AbstractSegments::getDynamicSymbols() const
{
    return m_dynSymbols;
}

std::string AbstractSegments::determineFamily() const
{
    const std::set<std::string>& files(getFiles());
    if (files.find("kaiten.c") != files.end() || files.find("kaiten2.c") != files.end())
    {
        return "ELF.Kaiten";
    }
    if (files.find("AES.cpp") != files.end())
    {
        return "ELF.AES";
    }
    if (files.find("ThreadMonGates.cpp") != files.end())
    {
        return "ELF.BillGates";
    }
    if (files.find("ThreadAttack.cpp") != files.end() || files.find("AtkThread.cpp") != files.end())
    {
        return "ELF.Elknot";
    }
    if (files.find("tshd.c") != files.end())
    {
        return "TinySH";
    }

    const std::vector<AbstractSymbol>& symbols(getAllSymbols());
    BOOST_FOREACH(const AbstractSymbol& symbol, symbols)
    {
        if (symbol.getType() == elf::symbol::k_function)
        {
           if (symbol.getName() == "tsunami")
           {
               return "ELF.Kaiten";
           }
        }
    }

    return "Undetermined";
}

std::string AbstractSegments::printSegment(boost::uint64_t p_offset) const
{
    BOOST_FOREACH(const SegmentType& segment, m_types)
    {
        if (segment.getOffset() == p_offset)
        {
            return segment.printToStdOut();
        }
    }

    if (m_dynamic.getOffset() != 0 && p_offset == m_dynamic.getOffset())
    {
        return m_dynamic.printToStdOut();
    }

    if (m_initArray.getOffset() != 0 && p_offset == m_initArray.getOffset())
    {
        return m_initArray.printToStd();
    }

    if (m_ctorsArray.getOffset() != 0 && p_offset == m_ctorsArray.getOffset())
    {
        return m_ctorsArray.printToStd();
    }

    return std::string();
}

std::string AbstractSegments::printToStdOut() const
{
    std::stringstream returnValue;
    returnValue << m_dynamic.printToStdOut();
    returnValue << m_dynSymbols.printToStdOut();
    returnValue << m_initArray.printToStd();

    BOOST_FOREACH(const SegmentType& seg, m_types)
    {
        returnValue << seg.printToStdOut();
    }

    BOOST_FOREACH(const Symbols& sym, m_otherSymbols)
    {
        returnValue << sym.printToStdOut();
    }

    return returnValue.str();
}
