#include "search_node.hpp"

#include <cstddef>

SearchNode::SearchNode() :
    m_storedData(),
    m_next(),
    m_failureNode(NULL)
{
}

SearchNode::~SearchNode()
{
}

SearchNode* SearchNode::getNext(const unsigned char p_value) const
{
    return m_next[p_value];
}

void SearchNode::setNext(const unsigned char p_value, SearchNode* p_node)
{
    m_next[p_value] = p_node;
}

const std::set<void*>& SearchNode::getStoredData() const
{
    return m_storedData;
}

void SearchNode::addReturnValues(const std::set<void*>& p_dataSet)
{
    m_storedData.insert(p_dataSet.begin(), p_dataSet.end());
}

void SearchNode::addReturnValue(void* p_data)
{
    m_storedData.insert(p_data);
}

SearchNode* SearchNode::getFailure() const
{
    return m_failureNode;
}

void SearchNode::setFailure(SearchNode* const p_node)
{
    m_failureNode = p_node;
}
