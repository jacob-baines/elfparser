#ifndef PROGRAM_HEADERS_HPP
#define PROGRAM_HEADERS_HPP

#include <vector>
#include <string>
#include <boost/cstdint.hpp>

#ifdef UNIT_TESTS
#include <gtest/gtest_prod.h>
#endif

class AbstractSegments;
class AbstractProgramHeader;

/*!
 * Parses the program header and contains the list of headers.
 * Passes information to AbstractSegments in order to create segments.
 */
class ProgramHeaders
{
public:

    //! Default initialize the member variables
    ProgramHeaders();

    //! Nothing of note
    ~ProgramHeaders();

    /*!
     * Parses the program header and stores entries in the program headers vector.
     * \param[in] p_data a pointer to the first entry
     * \param[in] p_count the number of expected entries
     * \param[in] p_size the expected size of a single entry
     * \param[in] p_is64 indicates if the binary is 64 bit or 32 bit
     * \param[in] p_isLE indicates if we are operating on a LE binary
     */
    void setHeaders(const char* p_data, boost::uint16_t p_count,
                    boost::uint16_t p_size, bool p_is64, bool p_isLE);

    /*!
     * Passes program header information into AbstractSegments for segment creation
     * \param[in,out] p_segments the segment object we'll pass info to
     */
    void extractSegments(AbstractSegments& p_segments);

    /*!
     * Scores the binary based on the entries that we expect to see
     * and the entries that are actually present.
     * \param[in,out] p_reasons store score / reason here
     */
    void evaluate(std::vector<std::pair<boost::int32_t, std::string> >& p_reasons) const;

    //! \return the program header vector
    const std::vector<AbstractProgramHeader>& getProgramHeaders() const;

    //! \return a string representation of the program header
    std::string printToStdOut() const;

private:

    // disable evil things
    ProgramHeaders(const ProgramHeaders& p_rhs);
    ProgramHeaders& operator=(const ProgramHeaders& p_rhs);

private:

    //! A list of the entries in the program header
    std::vector<AbstractProgramHeader> m_programHeaders;
};

#endif
