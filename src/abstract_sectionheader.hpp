#ifndef ABSTRACT_SECTION_HEADER_HPP
#define ABSTRACT_SECTION_HEADER_HPP

#include <string>
#include <vector>
#include <boost/cstdint.hpp>

#ifdef UNIT_TESTS
#include <gtest/gtest_prod.h>
#endif

namespace elf
{
    struct section_header_32;
    struct section_header_64;
}

/*!
 * Abstracts away the BE vs LE and 32 bit vs 64 bit stuff
 */
class AbstractSectionHeader
{
public:

    AbstractSectionHeader(const char* p_data, boost::uint16_t p_size,
                          const char* p_file, boost::uint64_t p_fileSize,
                          boost::uint8_t p_strIndex,
                          const std::vector<AbstractSectionHeader>& p_sections,
                          bool p_is64, bool p_isLE);
    AbstractSectionHeader(const AbstractSectionHeader& p_rhs);

    //! Nothing of note
    ~AbstractSectionHeader();

    bool is64() const;
    bool isLE() const;
    bool isExecutable() const;
    bool isWritable() const;
    std::string getName() const;
    std::string getTypeString() const;
    std::string getFlagsString() const;
    boost::uint32_t getType() const;
    boost::uint64_t getFlags() const;
    boost::uint64_t getVirtAddress() const;
    std::string getVirtAddressString() const;
    boost::uint64_t getPhysOffset() const;
    boost::uint64_t getSize() const;
    boost::uint32_t getLink() const;
    boost::uint32_t getInfo() const;
    boost::uint64_t getAddrAlign() const;
    boost::uint64_t getEntSize() const;

private:
    AbstractSectionHeader& operator=(const AbstractSectionHeader& p_rhs);

#ifdef UNIT_TESTS
private:
    FRIEND_TEST(LSTest, Sixtyfour_Intel_ls);
    FRIEND_TEST(LSTest, Thirtytwo_Intel_ls);
    FRIEND_TEST(LSTest, Thirtytwo_Arm_ls);
#endif

private:

    //! A reference to the list of section headers
    const std::vector<AbstractSectionHeader>& m_sections;

    //! This section header in 32 bit format
    const elf::section_header_32* m_section_header32;

    //! This section header in 64 bit format
    const elf::section_header_64* m_section_header64;

    //! The start of the file in memory
    const char* m_fileStart;

    //! The size of the file in memory
    boost::uint64_t m_size;

    //! Index of the string table
    boost::uint8_t m_strIndex;

    //! Indicates if the binary is 64 bit or not
    bool m_is64;

    //! Indicates if the bnary is LE or not
    bool m_isLE;
};

#endif
