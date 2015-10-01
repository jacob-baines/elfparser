#ifndef ABSTRACT_PROGRAM_HEADER_HPP
#define ABSTRACT_PROGRAM_HEADER_HPP

#include <vector>
#include <string>
#include <boost/cstdint.hpp>

namespace elf
{
    struct program_header_32;
    struct program_header_64;
}

class AbstractSegments;

/*!
 * Parses the program header. Abstracts 32 bit / 64 bit for any higher layer.
 * Passes information to AbstractSegments in order to create segments.
 */
class AbstractProgramHeader
{
public:

    /*!
     * Stores the program information in a way that is hidden from aller
     * \param[in] p_data a pointer to the first entry
     * \param[in] p_count the number of expected entries
     * \param[in] p_size the expected size of a single entry
     * \param[in] p_is64 indicates if the binary is 64 bit or 32 bit
     * \param[in] p_isLE indicates if the binary is little endian
     */
    AbstractProgramHeader(const char* p_data, boost::uint16_t p_size,
                          bool p_is64, bool p_isLE);
    AbstractProgramHeader(const AbstractProgramHeader& p_rhs);

    //! Nothing of note
    ~AbstractProgramHeader();

    bool is64() const;
    bool isLE() const;
    bool isExecutable() const;
    bool isWritable() const;
    std::string getFlagsString() const;
    std::string getName() const;
    boost::uint32_t getType() const;
    boost::uint64_t getOffset() const;
    boost::uint64_t getVirtualAddress() const;
    std::string getVirtualAddressString() const;
    boost::uint64_t getPhysicalAddress() const;
    std::string getPhysicalAddressString() const;
    boost::uint64_t getFileSize() const;
    boost::uint64_t getMemorySize() const;
    boost::uint32_t getFlags() const;

private:

    // disable evil things
    AbstractProgramHeader& operator=(const AbstractProgramHeader& p_rhs);

private:

    //! This program header in 32 bit format
    const elf::program_header_32* m_program_header32;

    //! This program header in 64 bit format
    const elf::program_header_64* m_program_header64;

    //! Indicates if the binary is 64 bit or not
    bool m_is64;

    //! Indicates if the bnary is LE or not
    bool m_isLE;

};

#endif
