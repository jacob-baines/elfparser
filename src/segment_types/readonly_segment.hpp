#ifndef READONLY_SEGMENT_HPP
#define READONLY_SEGMENT_HPP

#include "segment_type.hpp"

#include <set>
#include <vector>
#include <string>
#include <boost/cstdint.hpp>

/*!
 * This segment parses the read only segment looking for ascii strings
 */
class ReadOnlySegment : public SegmentType
{
public:

    /*!
     * Parses the read only segment looking for ascii strings
     * \param[in] p_start the start of the image
     * \param[in] p_offset the offset to this segment
     * \param[in] p_size the size of this segment
     * \param[in] p_type the type of this segment
     */
    ReadOnlySegment(const char* p_start, boost::uint32_t p_offset,
                    boost::uint32_t p_size, elf::section_type p_type);

    //! Nothing of note
    ~ReadOnlySegment();

    /*!
     * Looks for "evil" strings and add them to the reasons vector
     * \param[in] p_reasons the scoring vector to put the scores into
     * \param[in] unused capabilties set
     */
    void evaluate(std::vector<std::pair<boost::int32_t, std::string> >& p_reasons,
                  std::map<elf::Capabilties, std::set<std::string> >&) const;

    //! \return the string representation of the read only segment
    virtual std::string printToStdOut() const;

private:

    //! Disable evil things
    ReadOnlySegment(const ReadOnlySegment& p_rhs);
    ReadOnlySegment& operator=(const ReadOnlySegment& p_rhs);

private:

    //! The ascii strings in the read only segment
    std::set<std::string> m_asciiStrings;
};

#endif
