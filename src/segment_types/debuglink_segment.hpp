#ifndef DEBUGLINK_SEGMENT_HPP
#define DEBUGLINK_SEGMENT_HPP

#include "segment_type.hpp"

#include <string>
#include <boost/cstdint.hpp>

/*!
 * Holds the file name pointed at by the debug link. See more information here:
 * https://sourceware.org/gdb/onlinedocs/gdb/Separate-Debug-Files.html
 */
class DebugLinkSegment : public SegmentType
{
public:

    /*!
     * Puts the file name into m_file
     * \param[in] p_start the start of the binary
     * \param[in] p_offset the offset to this segment
     * \param[in] p_size the size of this segment
     * \param[in] p_type elf::k_progbits
     */
    DebugLinkSegment(const char* start, boost::uint32_t p_offset,
                     boost::uint32_t p_size, elf::section_type p_type);

    //! Nothing of note
    ~DebugLinkSegment();

    //! \return the string representation of the debug link
    virtual std::string printToStdOut() const;

private:

    //! Disable evil things
    DebugLinkSegment(const DebugLinkSegment& p_rhs);
    DebugLinkSegment& operator=(const DebugLinkSegment& p_rhs);

private:

    //! The filename the debug link points to
    std::string m_file;
};

#endif
