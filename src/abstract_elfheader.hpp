#ifndef ABSTRACT_ELFHEADER_HPP
#define ABSTRACT_ELFHEADER_HPP

#include <set>
#include <map>
#include <vector>
#include <string>
#include <cstddef>
#include <boost/cstdint.hpp>

namespace elf
{
    struct elf_header_32;
    struct elf_header_64;
}

#include "structures/capabilities.hpp"

/*!
 * Parses the ELF header and abstracts the 32/64 bit aspect away from
 * the caller.
 */
class AbstractElfHeader
{
public:

    //! Default initialize members
    AbstractElfHeader();

    //! Nothing of note
    ~AbstractElfHeader();

    /*!
     * Parses the ELF header
     * \param[in] p_data a pointer to the elf header
     * \param[in] p_size the total size of p_data
     */
    void setHeader(const char* p_data, std::size_t p_size);

    //! \return true if and only if the binary is 64 bit
    bool is64() const;

    //! \return the four magic bytes
    std::string getMagic() const;

    //! \return the class of the file (32 vs. 64 bit)
    std::string getClass() const;

    //! \return the encoding (le vs be)
    std::string getEncoding() const;

    //! \return the ELF file version
    std::string getFileVersion() const;

    //! \return the OS ABI
    std::string getOSABI() const;

    //! \return the ABI Version
    std::string getABIVersion() const;

    //! \return the binary type (exec, relocatable, etc)
    std::string getType() const;

    //! \return the flags
    std::string getFlags() const;

    //! \return the elf header size
    std::string getEHSize() const;

    //! \return the entry point
    std::string getEntryPointString() const;

    //! \return the elf version part 2
    std::string getVersion() const;

    //! \return the virtual address of the entry pointer
    boost::uint64_t getEntryPoint() const;

    //! \return the offset to the program header
    boost::uint32_t getProgramOffset() const;

    //! \return the number of expected program header entries
    boost::uint16_t getProgramCount() const;

    //! \return the size of a program header entry
    boost::uint16_t getProgramSize() const;

    //! \return the offset to the section header
    boost::uint32_t getSectionOffset() const;

    //! \return the size of a section table entry
    boost::uint16_t getSectionSize() const;

    //! \return the number of entries in the section header
    boost::uint16_t getSectionCount() const;

    //! \return the index of the section header string table
    boost::uint16_t getStringTableIndex() const;

    //! \return the offset of the section header string table
    boost::uint32_t getStringTableOffset(const char* p_start) const;

    //! \return an if there is le or not
    bool isLE() const;

    //! \return the architecture this file is made for
    std::string getMachine() const;

    //! \return a string representation of the elf header
    std::string printToStdOut() const;

    /*!
     * Examines the elf header results and stores information into one of the
     * two containers
     * \param[in,out] p_reasons the reasons for scoring vector
     * \param[in,out] p_capabilities the capabilities map
     */
    void evaluate(std::vector<std::pair<boost::int32_t, std::string> >& p_reasons,
                  std::map<elf::Capabilties, std::set<std::string> >& p_capabilities) const;

private:

    // disable evil things
    AbstractElfHeader(const AbstractElfHeader& p_rhs);
    AbstractElfHeader& operator=(const AbstractElfHeader& p_rhs);

private:

    //! Indicates if the binary is 64 bit or not
    bool m_is64;

    //! Stores the file size due to true.asm silliness
    boost::uint32_t m_fileSize;

    //! A pointer to the 32 bit version of the ELF header
    const elf::elf_header_32* m_header32;

    //! A pointer to the 64 bit version of the ELF header
    const elf::elf_header_64* m_header64;
};

#endif
