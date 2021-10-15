#ifndef ABSTRACT_DYNAMIC_HPP
#define ABSTRACT_DYNAMIC_HPP

#include <boost/cstdint.hpp>
#include <string>
#include <sstream>

#define CEXIT_SUCCESS " ";

class AbstractDynamicEntry
{
    private:

        // disable evil things
        // AbstractDynamicEntry& operator=(const AbstractDynamicEntry& p_rhs);

        boost::uint64_t m_tag;
        boost::uint64_t m_value;
        std::string m_stringValue;

    public:
        AbstractDynamicEntry(boost::uint64_t p_tag, boost::uint64_t p_value);
        AbstractDynamicEntry(const AbstractDynamicEntry& p_rhs);
        ~AbstractDynamicEntry();
        
        // string validate (m_stringValue) returns a buulean value if empty returns 1 if empty returns 0
        bool validString() const; 
        void createString(const char* p_strOffset);        
        std::string createTag(boost::uint64_t p_tag) const;
        std::string createValue(boost::uint64_t p_tag, boost::uint64_t p_value, const char *p_strTab) const;
        
        boost::uint64_t getTag() const;
        boost::uint64_t getValue() const;
        const std::string& getString() const;
    
        std::string printToStdOut() const;

};

#endif
