#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <string>
#include <boost/cstdint.hpp>

class Segment
{
public:
    Segment(const std::string& p_name, const std::string& p_type,
            boost::uint64_t p_physOffset, boost::uint64_t p_virtAddress,
            boost::uint64_t p_size, boost::uint32_t p_link,
            bool p_executable, bool p_writable,
            bool p_isDynamic);
    ~Segment();

    boost::uint64_t getVirtAddress() const;
    boost::uint64_t getSize() const;
    boost::uint64_t getPhysOffset() const;
    boost::uint32_t getLink() const;
    const std::string& getName() const;
    const std::string& getType() const;
    bool isDynamic() const;

private:
    //! disable evil things
    Segment& operator=(const Segment& p_rhs);
private:

    //! the name of this segment
    std::string m_name;

    //! the type of this segment
    std::string m_type;

    //! the file offset to this segment
    boost::uint64_t m_physOffset;

    //! the virtual address of this segment
    boost::uint64_t m_virtAddress;

    //! size on file
    boost::uint64_t m_size;

    //! the link to another table that may exist
    boost::uint32_t m_link;

    //! indicates if the area is executable
    bool m_executable;

    //! indicates if this area is writable
    bool m_writable;

    //! indicates if this is the dynamic section
    bool m_isDynamic;
};

#endif
