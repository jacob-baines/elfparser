#ifndef SECTIONHEADERS_HPP
#define SECTIONHEADERS_HPP

#include <map>
#include <set>
#include <string>
#include <vector>
#include <boost/cstdint.hpp>

#include "structures/capabilities.hpp"

class AbstractSegments;
class AbstractSectionHeader;

/*!
 * Parses the section header and contains the list of headers.
 * Passes information to AbstractSegments in order to create segments.
 */
class SectionHeaders
{
public:

    //! Default initialize the member variables
    SectionHeaders();

    //! Nothing of note
    ~SectionHeaders();

    /*!
     * Parses the section header and pushes the various entries into the
     * headers vector.
     * \param[in] p_data a pointer to the first section entry
     * \param[in] p_start a pointer to the start of the file
     * \param[in] p_total_size the total size of the file
     * \param[in] p_count the number of entries expected in the header
     * \param[in] p_size the size of section header entry
     * \param[in] p_stringIndex the index of the section header string table
     * \param[in] p_is64 indicates if the binary is 32 bit or 64 bit
     * \param[in] p_isLE indicates if the binary is LE or BE
     * \param[in,out] p_reasons scoring reasons
     */
    void setHeaders(const char* p_data, const char* p_start,
                    boost::uint64_t p_total_size, boost::uint16_t p_count,
                    boost::uint16_t p_size, std::size_t p_stringIndex,
                    bool p_is64, bool p_isLE,
                    std::map<elf::Capabilties, std::set<std::string> >& p_capabilities);

    /*!
     * Passes section header information into AbstractSegments for segment creation
     * \param[in,out] p_segments the segment object we'll pass info to
     */
    void extractSegments(AbstractSegments& p_segments);

    /*!
     * Calls into the various segments for evaluation / scoring information.
     * \param[in,out] p_reasons stores the scoring and reasons
     * \param[in,out] p_capabilities stores information about what the binary does
     */
    void evaluate(std::vector<std::pair<boost::int32_t, std::string> >& p_reasons,
                  std::map<elf::Capabilties, std::set<std::string> >& p_capabilities) const;

    //! \return the section headers
    const std::vector<AbstractSectionHeader>& getSections() const;

    //! \return the string table index
    boost::uint32_t getStringTableIndex() const;

    //! \return a string representation of the section headers
    std::string printToStdOut() const;

private:

    //! disable evil things
    SectionHeaders(const SectionHeaders& p_rhs);
    SectionHeaders& operator=(const SectionHeaders& p_rhs);

private:

    //! A list of the entries in the program header
    std::vector<AbstractSectionHeader> m_sectionHeaders;

    //! Total size of the binary
    boost::uint32_t m_totalSize;

    //! The index of the strings in the sections
    boost::uint32_t m_stringIndex;
};

#endif
