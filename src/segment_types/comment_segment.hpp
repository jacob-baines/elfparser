#ifndef COMMENT_SEGMENT_HPP
#define COMMENT_SEGMENT_HPP

#include "segment_type.hpp"

#include <string>
#include <boost/cstdint.hpp>

/*!
 * The comment segment. Holds pretty generic information. Example of the
 * contents of a comment segment:
 * "GCC: (Ubuntu/Linaro 4.7.3-1ubuntu1) 4.7.3"
 */
class CommentSegment : public SegmentType
{
public:

    /*!
     * Puts the comment into m_comment
     * \param[in] p_start the start of the binary
     * \param[in] p_offset the offset to this segment
     * \param[in] p_size the size of this segment
     * \param[in] p_type elf::k_progbits
     */
    CommentSegment(const char* p_start, boost::uint32_t p_offset,
                   boost::uint32_t p_size, elf::section_type p_type);

    //! Nothing of note
    ~CommentSegment();

    //! \return the string representation of the comment segment
    virtual std::string printToStdOut() const;

private:

    //! disable evil things
    CommentSegment(const CommentSegment& p_rhs);
    CommentSegment& operator=(const CommentSegment& p_rhs);

private:

    //! The strings that the comment holds
    std::string m_comment;
};

#endif
