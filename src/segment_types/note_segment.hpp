#ifndef NOTE_SEGMENT_HPP
#define NOTE_SEGMENT_HPP

#include "segment_type.hpp"

#include <string>
#include <boost/cstdint.hpp>

namespace elf
{
    struct note;
}

/*!
 * Parses the note segment and tries to interpret the contents. Some parsed
 * examples:
 * "Name="GNU, Type=1, Type String="NT_GNU_ABI_TAG", Description="OS Linux 2.6.24""
 * "Name="GNU", Type=3, Type String="NT_GNU_BUILD_ID", Description="f21bf131b6da48c9bfe35b117c5b15a687174362""
 */
class NoteSegment : public SegmentType
{
public:

    /*!
     * Parses the note and stores the values into the member variables
     * \param[in] p_start the start of the binary
     * \param[in] p_offset the offset to this segment
     * \param[in] p_size the size of this segment
     * \param[in] p_type elf::k_note
     */
    NoteSegment(const char* start, boost::uint32_t p_offset,
                boost::uint32_t p_size, elf::section_type p_type);

    //nothing of note (lol)
    ~NoteSegment();

    //! \return the string representation of the note segment
    virtual std::string printToStdOut() const;

private:

    //!disable evil things
    NoteSegment(const NoteSegment& p_rhs);
    NoteSegment& operator=(const NoteSegment& p_rhs);

private:

    //! A pointer to the start of the note structure
    const elf::note* m_note;

    //! The name of the note
    std::string m_name;

    //! The interpreted type of the note
    std::string m_noteType;

    //! The interpreted description of the note
    std::string m_description;
};

#endif
