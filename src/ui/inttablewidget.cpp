#ifdef QT_GUI
#include "inttablewidget.hpp"

#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>

IntWidgetItem::IntWidgetItem(boost::uint64_t p_value, bool p_hex) :
    QTableWidgetItem(),
    m_value(p_value)
{
    if (!p_hex)
    {
        QString intValue(boost::lexical_cast<std::string>(m_value).c_str());
        setText(intValue);
    }
    else
    {
        std::stringstream value;
        value << "0x" << std::hex << p_value;
        QString intValue(value.str().c_str());
        setText(intValue);
    }
}

IntWidgetItem::~IntWidgetItem()
{
}

bool IntWidgetItem::operator <(const QTableWidgetItem& other) const
{
    const IntWidgetItem& otherCasted = dynamic_cast<const IntWidgetItem&>(other);
    return m_value < otherCasted.m_value;
}
#endif
