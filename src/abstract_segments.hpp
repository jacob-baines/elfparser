#ifndef ABSTRACT_SEGMENTS_HPP
#define ABSTRACT_SEGMENTS_HPP

#include <map>
#include <set>
#include <vector>
#include <string>
#include <cstddef>
#include <boost/cstdint.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "symbols.hpp"
#include "initarray.hpp"
#include "dynamicsection.hpp"
#include "segment_types/segment_type.hpp"
#include "structures/capabilities.hpp"

class AbstractSectionHeader;
class AbstractProgramHeader;
class Segment;

/*!
 * Creates Segment objects based on the passed in program headers and/or the
 * section headers. Because ELF is sketchy the section headers don't have to be
 * present and they are a big target for people to insert fake data.
 */
class AbstractSegments
{
public:

    //! Default initializes the member variables
    AbstractSegments();

    //! Releases the segments
    ~AbstractSegments();

    std::set<std::string> getFiles() const;

    void setStart(const char* p_data, boost::uint32_t p_size,
                  bool p_is64, bool p_isLE, bool p_isDY);

    void makeSegmentFromSectionHeader(const AbstractSectionHeader& p_segment);

    void makeSegmentFromProgramHeader(const AbstractProgramHeader& p_header);

    void createDynamic();
    void generateSegments();

    //! \return the base address
    boost::uint64_t getBaseAddress() const;

    boost::uint64_t getOffsetFromVirt(boost::uint64_t p_virtual) const;

    /*!
     * Calls into the various segments for evaluation / scoring information.
     * \param[in,out] p_reasons stores the scoring and reasons
     * \param[in,out] p_capabilities stores information about what the binary does
     */
    void evaluate(std::vector<std::pair<boost::int32_t, std::string> >& p_reasons,
                  std::map<elf::Capabilties, std::set<std::string> >& p_capabilities) const;

    /*!
     *  Tries to guess which family of malware the binary is part of - if at all
     */
    std::string determineFamily() const;

    //! \return the string at the given offset
    std::string printSegment(boost::uint64_t p_offset) const;

    //! \return a string version of all segments. Name is a bit misleading.
    std::string printToStdOut() const;

    std::vector<AbstractSymbol> getAllSymbols() const;
    const DynamicSection& getDynamicSection() const;
    const Symbols& getDynamicSymbols() const;

private:

    //! Disable evil things
    AbstractSegments(const AbstractSegments& p_rhs);
    AbstractSegments& operator=(const AbstractSegments& p_rhs);

private:

    //! the start of the file in memory
    const char* m_data;

    //! the size of the file in memory
    boost::uint32_t m_size;

    //! All the section segments
    std::vector<Segment> m_sections;

    //! All the program segments
    std::vector<Segment> m_programs;

    //! All the sections/programs converted to subtypes
    boost::ptr_vector<SegmentType> m_types;

    //! All the offsets so we don't parse the same thing twice
    std::set<const char*> m_offsets;

    //! The base address. Taken from the first PT_LOAD
    boost::uint64_t m_baseAddress;

    //! A fully populated dynamic section
    DynamicSection m_dynamic;

    //! A populated list of the symbols
    Symbols m_dynSymbols;

    //! Other symbols
    boost::ptr_vector<Symbols> m_otherSymbols;

    //! the init array
    InitArray m_initArray;

    //! the ctors array
    InitArray m_ctorsArray;

    //! indicates if we set the base address yet
    bool m_setBase;

    //! indicates if this is 64 bit
    bool m_is64;

    //! indicates if this is LE
    bool m_isLE;

    //! indicates if this is an so or executable
    bool m_isDY;

    //! indicates if we detected a fake dynamic string table
    bool m_fakeDynamicStringTable;

    //! indicates if we detected a valid string table section header with no virt addr
    bool m_strTabMissingVirtAddress;
};

#endif
