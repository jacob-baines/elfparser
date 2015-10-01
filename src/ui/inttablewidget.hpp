#ifdef QT_GUI
#ifndef INTENTRY_HPP
#define INTENTRY_HPP

#include <QTableWidgetItem>
#include <boost/cstdint.hpp>

class IntWidgetItem : public QTableWidgetItem
{
public:
    IntWidgetItem(boost::uint64_t p_value, bool p_hex = false);
    ~IntWidgetItem();

    bool operator <(const QTableWidgetItem &other) const;

private:
    //! the underlying value in int format
    boost::uint64_t m_value;
};

#endif
#endif
