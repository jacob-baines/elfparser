#ifndef SYMBOLS_HPP
#define SYMBOLS_HPP

#include <set>
#include <map>
#include <vector>
#include <string>
#include <boost/cstdint.hpp>

#ifdef UNIT_TESTS
#include <gtest/gtest_prod.h>
#endif

#include "abstract_symbol.hpp"
#include "structures/capabilities.hpp"

class AbstractSegments;

class Symbols
{
public:
    Symbols();
    ~Symbols();

    void createSymbols(const char* p_data, boost::uint64_t p_dataSize, boost::uint64_t p_symTabOffset,
                       boost::uint32_t p_symTabSize, boost::uint64_t p_strTabOffset,
                       boost::uint64_t p_strTableSize,
                       const AbstractSegments& p_segments,
                       bool p_is64, bool p_isLE, bool p_isDY);

    const std::vector<AbstractSymbol>& getSymbols() const;

    //! find a symbol based on a passed in address
    std::string findSymbol(boost::uint64_t p_address) const;

    //! return the set of files found in the symbols
    std::set<std::string> getFiles() const;

    /*!
     * Calls into the various segments for evaluation / scoring information.
     * \param[in,out] p_reasons stores the scoring and reasons
     * \param[in,out] p_capabilities stores information about what the binary does
     */
    void evaluate(std::vector<std::pair<boost::int32_t, std::string> >& p_reasons,
                  std::map<elf::Capabilties, std::set<std::string> >& p_capabilities) const;

    std::string printToStdOut() const;

#ifdef UNIT_TESTS
private:
    FRIEND_TEST(LSTest, Sixtyfour_Intel_ls);
    FRIEND_TEST(LSTest, Thirtytwo_Intel_ls);
    FRIEND_TEST(LSTest, Thirtytwo_Arm_ls);
#endif

private:

    //! indicates if the executable is an executable
    bool m_isDY;

    //! Contains a vector
    std::vector<AbstractSymbol> m_symbols;

    //! Contains the file names listed in the symbols
    std::set<std::string> m_files;
};

#endif
