#ifndef STRINGTABLE_SEGMENT_HPP
#define STRINGTABLE_SEGMENT_HPP

#include "segment_type.hpp"

#include <set>
#include <string>
#include <boost/cstdint.hpp>

/*!
 * Holds all the strings in the string table
 */
class StringTableSegment : public SegmentType
{
public:

    /*!
     * Parses the string table for strings and stores them in the m_strings set
     * \param[in] p_start the start of the image
     * \param[in] p_offset the offset to this segment
     * \param[in] p_size the size of the segment
     * \param[in] p_type the type of segment
     */
    StringTableSegment(const char* p_start, boost::uint32_t p_offset,
                       boost::uint32_t p_size, elf::section_type p_type);

    //! nothing of note
    ~StringTableSegment();

    //! \return the number of strings in m_strings
    std::size_t getSize() const;

    /*!
     * Looks up a string in the string table and demangles it (Linux only)
     * \param[in] p_index the index of the string to look up
     * \return the string in the table
     */
    std::string stringLookup(std::size_t p_index) const;

    //! \return a string representation of the String Table
    virtual std::string printToStdOut() const;

private:

    //! Disable evil things
    StringTableSegment(const StringTableSegment& p_rhs);
    StringTableSegment& operator=(const StringTableSegment& p_rhs);

private:

    //! A set of the strings in the segment
    std::set<std::string> m_stringsSet;

    //! The start of the segment
    const char* m_start;
};

#endif
