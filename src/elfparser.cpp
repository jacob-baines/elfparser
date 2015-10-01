#include "elfparser.hpp"

#include "structures/elfheader.hpp"
#include "datastructures/search_value.hpp"

#include <md5.h>
#include <sha1.h>
#include <sha256.h>
#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

namespace
{
    std::size_t findFileSize(const std::string& p_file)
    {
        std::ifstream in(p_file.c_str(), std::ios::binary | std::ios::ate);
        if (!in.is_open())
        {
            throw std::runtime_error("Could not open " + p_file);
        }
        if (!in.good())
        {
            throw std::runtime_error("Error opening " + p_file);
        }
        std::size_t return_value = in.tellg();
        in.close();
        return return_value;
    }
}

ELFParser::ELFParser() :
    m_score(0),
    m_elfHeader(),
    m_programHeader(),
    m_sectionHeader(),
    m_segments(),
    m_mapped_file(),
    m_reasons(),
    m_capabilities(),
    m_filename(),
    m_fileSize(0),
    m_searchEngine(),
    m_searchValues()
{
    m_searchValues.push_back(new SearchValue("UPX!", elf::k_packed, "UPX signature found"));
    m_searchValues.push_back(new SearchValue("the UPX Team. All Rights Reserved", elf::k_packed, "UPX copyright string found"));
    m_searchValues.push_back(new SearchValue("PRIVMSG ", elf::k_irc, "IRC command PRIVMSG found"));
    m_searchValues.push_back(new SearchValue("JOIN ", elf::k_irc, "IRC command JOIN found"));
    m_searchValues.push_back(new SearchValue("NOTICE ", elf::k_irc, "IRC command NOTICE found"));
    m_searchValues.push_back(new SearchValue("ustar\0", elf::k_compression, "Tar Archive signature found"));
    m_searchValues.push_back(new SearchValue("\x1f\x8b\x08", elf::k_compression, "Gzip signature found"));
    m_searchValues.push_back(new SearchValue("inflate 1.1.4 Copyright 1995-2002 Mark Adler", elf::k_compression, "Inflate 1.1.4 copyright string"));
    m_searchValues.push_back(new SearchValue("inflate 1.2.3 Copyright 1995-2005 Mark Adler", elf::k_compression, "Inflate 1.2.3 copyright string"));
    m_searchValues.push_back(new SearchValue("inflate 1.2.8 Copyright 1995-2013 Mark Adler", elf::k_compression, "Inflate 1.2.8 copyright string"));
    m_searchValues.push_back(new SearchValue("/proc/cpuinfo",  elf::k_infoGathering, "Examines /proc/cpuinfo"));
    m_searchValues.push_back(new SearchValue("/proc/meminfo",  elf::k_infoGathering, "Examines /proc/meminfo"));
    m_searchValues.push_back(new SearchValue("/proc/stat", elf::k_infoGathering, "Examines /proc/stat"));
    BOOST_FOREACH(SearchValue& value, m_searchValues)
    {
        m_searchEngine.addWord(value.m_search, &value);
    }

    m_searchEngine.compile();
}

ELFParser::~ELFParser()
{
    m_mapped_file.close();
}

boost::uint32_t ELFParser::getScore() const
{
    return m_score;
}

std::string ELFParser::getFilename() const
{
    return m_filename;
}

std::size_t ELFParser::getFileSize() const
{
    return m_fileSize;
}

std::string ELFParser::getSha1() const
{
    SHA1 sha1;
    return sha1(m_mapped_file.data(), m_fileSize);
}

std::string ELFParser::getSha256() const
{
    SHA256 sha256;
    return sha256(m_mapped_file.data(), m_fileSize);
}

std::string ELFParser::getMD5() const
{
    MD5 md5;
    return md5(m_mapped_file.data(), m_fileSize);
}

std::string ELFParser::getFamily() const
{
    return m_segments.determineFamily();
}

const std::vector<std::pair<boost::int32_t, std::string> >& ELFParser::getReasons() const
{
    return m_reasons;
}

void ELFParser::parse(const std::string& p_file)
{
    if (p_file.empty())
    {
        throw std::runtime_error("Parser given an empty file name.");
    }

    m_fileSize = findFileSize(p_file);
    m_mapped_file.open(p_file, m_fileSize);
    if (!m_mapped_file.is_open())
    {
        throw std::runtime_error("Failed to memory map the file.");
    }
    else
    {
        m_filename.assign(p_file);
    }

    m_elfHeader.setHeader(m_mapped_file.data(), m_fileSize);
    m_programHeader.setHeaders(m_mapped_file.data() +
        m_elfHeader.getProgramOffset(), m_elfHeader.getProgramCount(),
        m_elfHeader.getProgramSize(), m_elfHeader.is64(), m_elfHeader.isLE());
    m_sectionHeader.setHeaders(m_mapped_file.data() +
        m_elfHeader.getSectionOffset(), m_mapped_file.data(), m_fileSize,
        m_elfHeader.getSectionCount(), m_elfHeader.getSectionSize(),
        m_elfHeader.getStringTableIndex(), m_elfHeader.is64(), m_elfHeader.isLE(),
        m_capabilities);
    m_segments.setStart(m_mapped_file.data(), m_fileSize, m_elfHeader.is64(),
                        m_elfHeader.isLE(), m_elfHeader.getType() == "ET_DYN");

    // important to do section header first since it produces more complete data
    // create "segments" based off of the section header and program header
    m_sectionHeader.extractSegments(m_segments);
    m_programHeader.extractSegments(m_segments);
    m_segments.createDynamic();
    m_segments.generateSegments();
}

void ELFParser::evaluate()
{
    m_elfHeader.evaluate(m_reasons, m_capabilities);
    m_programHeader.evaluate(m_reasons);
    m_sectionHeader.evaluate(m_reasons, m_capabilities);
    m_segments.evaluate(m_reasons, m_capabilities);

    std::set<void*> results = m_searchEngine.search(m_mapped_file.data(), m_fileSize);
    BOOST_FOREACH(void* result, results)
    {
        SearchValue* converted = static_cast<SearchValue*>(result);
        m_capabilities[converted->m_type].insert(converted->m_info);
    }

    regexScan();

    findELF();

    for (std::map<elf::Capabilties, std::set<std::string> >::const_iterator it = m_capabilities.begin();
         it != m_capabilities.end(); ++it)
    {
        switch(it->first)
        {
            case elf::k_fileFunctions:
                break;
            case elf::k_networkFunctions:
                m_reasons.push_back(std::make_pair(1 * it->second.size(), std::string("Network functions")));
                break;
            case elf::k_processManipulation:
                m_reasons.push_back(std::make_pair(4 * it->second.size(), std::string("Process manipulation functions")));
                break;
            case elf::k_pipeFunctions:
                break;
            case elf::k_crypto:
                break;
            case elf::k_infoGathering:
                m_reasons.push_back(std::make_pair(1 * it->second.size(), std::string("Information gathering")));
                break;
            case elf::k_envVariables:
                m_reasons.push_back(std::make_pair(2 * it->second.size(), std::string("Environment variable manipulation")));
                break;
            case elf::k_permissions:
                break;
            case elf::k_syslog:
                m_reasons.push_back(std::make_pair(1 * it->second.size(), std::string("Syslog manipulation functions")));
                break;
            case elf::k_packetSniff:
                m_reasons.push_back(std::make_pair(10 * it->second.size(), std::string("Packet sniffing")));
                break;
            case elf::k_shell:
                m_reasons.push_back(std::make_pair(10 * it->second.size(), std::string("Shell commands")));
                break;
            case elf::k_packed:
                m_reasons.push_back(std::make_pair(25 * it->second.size(), std::string("Packed")));
                break;
            case elf::k_irc:
                m_reasons.push_back(std::make_pair(15 * it->second.size(), std::string("IRC functionality")));
                break;
            case elf::k_http:
                m_reasons.push_back(std::make_pair(4 * it->second.size(), std::string("HTTP functionality")));
                break;
            case elf::k_compression:
                m_reasons.push_back(std::make_pair(4 * it->second.size(), std::string("Compression")));
                break;
            case elf::k_ipAddress:
                m_reasons.push_back(std::make_pair(4 * it->second.size(), std::string("Hard coded IPv4 addresses")));
                break;
            case elf::k_url:
                break;
            case elf::k_hooking:
                m_reasons.push_back(std::make_pair(25 * it->second.size(), std::string("Function hooking")));
                break;
            case elf::k_antidebug:
                m_reasons.push_back(std::make_pair(10 * it->second.size(), std::string("Anti debug techniques")));
                break;
            case elf::k_dropper:
                m_reasons.push_back(std::make_pair(50 * it->second.size(), std::string("Dropper functionaltiy")));
                break;
            default:
                break;
        }
    }

    for (std::vector<std::pair<boost::int32_t, std::string> >::const_iterator it = m_reasons.begin();
         it != m_reasons.end(); ++it)
    {
        m_score += it->first;
    }
}

const AbstractElfHeader& ELFParser::getElfHeader() const
{
    return m_elfHeader;
}

const ProgramHeaders& ELFParser::getProgramHeaders() const
{
    return m_programHeader;
}

const SectionHeaders& ELFParser::getSectionHeaders() const
{
    return m_sectionHeader;
}

const AbstractSegments& ELFParser::getSegments() const
{
    return m_segments;
}

const DynamicSection& ELFParser::getDynamicSection() const
{
    return m_segments.getDynamicSection();
}

void ELFParser::printReasons() const
{
    std::cout << "---- Scoring Reasons ----" << std::endl;
    for (std::vector<std::pair<boost::int32_t, std::string> >::const_iterator it = m_reasons.begin();
         it != m_reasons.end(); ++it)
    {
        std::cout << it->first << " -> " << it->second << std::endl;
    }
}

void ELFParser::printCapabilities() const
{
    std::cout << "---- Detected Capabilities ----" << std::endl;
    for (std::map<elf::Capabilties, std::set<std::string> >::const_iterator it = m_capabilities.begin();
         it != m_capabilities.end(); ++it)
    {
        switch (it->first)
        {
            case elf::k_fileFunctions:
                std::cout << "\tFile Functions" << std::endl;
                break;
            case elf::k_networkFunctions:
                std::cout << "\tNetwork Functions" << std::endl;
                break;
            case elf::k_processManipulation:
                std::cout << "\tProcess Manipulation" << std::endl;
                break;
            case elf::k_pipeFunctions:
                std::cout << "\tPipe Functions" << std::endl;
                break;
            case elf::k_crypto:
                std::cout << "\tRandom Functions" << std::endl;
                break;
            case elf::k_infoGathering:
                std::cout << "\tInformation Gathering" << std::endl;
                break;
            case elf::k_envVariables:
                std::cout << "\tEnvironment Variables" << std::endl;
                break;
            case elf::k_permissions:
                std::cout << "\tPermissions" << std::endl;
                break;
            case elf::k_syslog:
                std::cout << "\tSystem Log" << std::endl;
                break;
            case elf::k_packetSniff:
                std::cout << "\tPacket Sniffing" << std::endl;
                break;
            case elf::k_shell:
                std::cout << "\tShell" << std::endl;
                break;
            case elf::k_packed:
                std::cout << "\tPacked" << std::endl;
                break;
            case elf::k_irc:
                std::cout << "\tIRC" << std::endl;
                break;
            case elf::k_http:
                std::cout << "\tHTTP" << std::endl;
                break;
            case elf::k_compression:
                std::cout << "\tCompression" << std::endl;
                break;
            case elf::k_ipAddress:
                std::cout << "\tIP Addresses" << std::endl;
                break;
            case elf::k_url:
                std::cout << "\tURL" << std::endl;
                break;
            case elf::k_hooking:
                std::cout << "\tFunction Hooking" << std::endl;
                break;
            case elf::k_antidebug:
                std::cout << "\tAnti-Debug" << std::endl;
                break;
            case elf::k_dropper:
                std::cout << "\tDropper" << std::endl;
                break;
            default:
                std::cout << "\tUnassigned" << std::endl;
                break;
        }
        BOOST_FOREACH(const std::string& info, it->second)
        {
            std::cout << "\t\t" << info << std::endl;
        }
    }
}

void ELFParser::printAll() const
{
    std::cout << "---- ELF Structures ----" << std::endl;
    std::cout << m_elfHeader.printToStdOut();
    std::cout << m_programHeader.printToStdOut();
    std::cout << m_sectionHeader.printToStdOut();
    std::cout << m_segments.printToStdOut() << std::endl;
}

const std::map<elf::Capabilties, std::set<std::string> >& ELFParser::getCapabilties() const
{
    return m_capabilities;
}

void ELFParser::regexScan()
{
    try
    {
        // ips
        boost::regex pattern("[1-2]?[0-9]?[0-9]\\.[1-2]?[0-9]?[0-9]\\.[1-2]?[0-9]?[0-9]\\.[1-2]?[0-9]?[0-9](?:[:0-9]{2,})*");
        const char* start = m_mapped_file.data();
        boost::cmatch m;
        while (boost::regex_search(start, m_mapped_file.data() + m_fileSize,m,pattern))
        {
            for (auto x:m)
            {
                m_capabilities[elf::k_ipAddress].insert(x);
            }
            start = (m_mapped_file.data() + m_fileSize) - m.suffix().length();
        }

        // urls
        boost::regex urlPattern("(?:(?:http|https)://[A-Za-z0-9_./:%+?+)|(?:www.[A-Za-z0-9/:]+\\.com)");
        start = m_mapped_file.data();
        while (boost::regex_search(start, m_mapped_file.data() + m_fileSize,m,urlPattern))
        {
            for (auto x:m)
            {
                m_capabilities[elf::k_url].insert(x);
            }
            start = (m_mapped_file.data() + m_fileSize) - m.suffix().length();
        }

        // commands
        boost::regex shellPattern("(?:(?:wget|chmod|killall|nohup) [[:print:]]+)|(?:tar -[[:print:]]+)");
        start = m_mapped_file.data();
        while (boost::regex_search(start, m_mapped_file.data() + m_fileSize,m,shellPattern))
        {
            for (auto x:m)
            {
                m_capabilities[elf::k_shell].insert(x);
            }
            start = (m_mapped_file.data() + m_fileSize) - m.suffix().length();
        }

        // url request
        boost::regex urlRequest("(?:POST (?:/|%s)|GET (?:/|%s)|CONNECT (?:/|%s)|User-Agent:)[[:print:]]+");
        start = m_mapped_file.data();
        while (boost::regex_search(start, m_mapped_file.data() + m_fileSize,m,urlRequest))
        {
            for (auto x:m)
            {
                m_capabilities[elf::k_http].insert(x);
            }
            start = (m_mapped_file.data() + m_fileSize) - m.suffix().length();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void ELFParser::findELF()
{
    SearchTree elfSearch;
    elfSearch.addWord("\x7f\x45\x4c\x46", this);
    elfSearch.compile();
    std::set<const char*> data = elfSearch.findOffsets(this->m_mapped_file.data() + 1, m_fileSize - 1);
    BOOST_FOREACH(const char* fib, data) {
        try
        {
            AbstractElfHeader newHeader;
            newHeader.setHeader(fib, fib - (m_mapped_file.data() + m_fileSize));
            if ((fib + newHeader.getProgramOffset()) < (m_mapped_file.data() + m_fileSize))
            {
                std::stringstream binaryFound;
                binaryFound << "Embedded ELF binary found at file offset 0x"
                            << std::hex << fib - m_mapped_file.data()
                            << " (" << std::dec << fib - m_mapped_file.data() << ")";
                m_capabilities[elf::k_dropper].insert(binaryFound.str());
            }
        }
        catch (std::exception& e) {
            // do nothing
        }
    }
}
