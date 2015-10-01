#ifndef ELF_PARSER
#define ELF_PARSER

#include <map>
#include <set>
#include <utility>
#include <vector>
#include <string>
#include <boost/cstdint.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include "abstract_elfheader.hpp"
#include "programheaders.hpp"
#include "sectionheaders.hpp"
#include "abstract_segments.hpp"
#include "programheaders.hpp"
#include "structures/capabilities.hpp"
#include "datastructures/search_tree.hpp"

class SearchValue;

/*!
 * Parses an ELF binary and computes a score that indicates how malicious
 * or dangerous the binary is. A lot of good information on parsing
 * an ELF file can be found here:
 * http://man7.org/linux/man-pages/man5/elf.5.html#DESCRIPTION_
 */
class ELFParser
{
public:

    //! Does nothing except default initialization of all members
    ELFParser();

    //! Closes the mapped file and therefore invalidates all data
    ~ELFParser();

    /*!
     * Maps the file into memory and passes the data around to the
     * various parsers. Creates all the segment objects and builds
     * the symbol tables at the end.
     * \param[in] p_file the file to analyze
     * \throw if we can't parse the file
     */
    void parse(const std::string& p_file);

    /*!
     * Asks the various parsers to "score" their portion of the binary.
     * Also, capabilities information is populated by the segments.
     */
    void evaluate();

    //! \return the binaries score
    boost::uint32_t getScore() const;

    //! prints the various elf structures to standard out
    void printAll() const;

    //! prints the scoring reasons to standard out
    void printReasons() const;

    //! prints the binaries capabilities to standard out
    void printCapabilities() const;

    //! \return the file name that was just parsed
    std::string getFilename() const;

    //! \return the size of the file in bytes
    std::size_t getFileSize() const;

    //! \return sha1 of the file
    std::string getSha1() const;

    //! \return sha256 of the file
    std::string getSha256() const;

    //! \return md5 of the file
    std::string getMD5() const;

    //! \return the vector of scoring reasons
    const std::vector<std::pair<boost::int32_t, std::string> >& getReasons() const;

    //! \return the capabilties map
    const std::map<elf::Capabilties, std::set<std::string> >& getCapabilties() const;

    //! \return a const reference to the elf header
    const AbstractElfHeader& getElfHeader() const;

    //! \return a const reference to the program headers
    const ProgramHeaders& getProgramHeaders() const;

    //! \return a const reference to the section headers
    const SectionHeaders& getSectionHeaders() const;

    //! \return a const reference to the abstract semgnents
    const AbstractSegments& getSegments() const;

    //! \return a const reference to the dynamic section
    const DynamicSection& getDynamicSection() const;

    //! \return a string indicating the malware family
    std::string getFamily() const;

private:

    //! Scans the binary with pre defined regular expressions
    void regexScan();

    //! Scans the binary looking for an ELF header
    void findELF();

private:

    //! The binaries score
    boost::uint32_t m_score;

    //! The object that parses the elf header
    AbstractElfHeader m_elfHeader;

    //! The object that parses the program header
    ProgramHeaders m_programHeader;

    //! The object that parses the section header
    SectionHeaders m_sectionHeader;

    //! The object that creates and stores the various segments
    AbstractSegments m_segments;

    //! The memory mapped file
    boost::iostreams::mapped_file_source m_mapped_file;

    //! A log (of sorts) of the scoring
    std::vector<std::pair<boost::int32_t, std::string> > m_reasons;

    //! Information about things the binary can do (ie, file manipulation)
    std::map<elf::Capabilties, std::set<std::string> > m_capabilities;

    //! The file that was analyzed
    std::string m_filename;

    //! The size of the analyzed file
    std::size_t m_fileSize;

    //! The aho corasick search engine
    SearchTree m_searchEngine;

    //! The ptr vector to hold the search engine values
    boost::ptr_vector<SearchValue> m_searchValues;
};

#endif
