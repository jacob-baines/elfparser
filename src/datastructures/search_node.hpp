#ifndef ELFPARSER_SEARCHNODE_HPP
#define ELFPARSER_SEARCHNODE_HPP

#include <set>

class SearchNode
{
public:

    SearchNode();
    ~SearchNode();

    SearchNode* getNext(const unsigned char p_value) const;

    std::size_t getNextSize() const { return sizeof(m_next) / sizeof(void*); }

    void setNext(const unsigned char p_value, SearchNode* p_node);

    const std::set<void*>& getStoredData() const;
    void addReturnValues(const std::set<void*>& p_dataSet);
    void addReturnValue(void* p_data);

    SearchNode* getFailure() const;
    void setFailure(SearchNode* const p_node);

private:

    SearchNode(const SearchNode& p_rhs);
    SearchNode& operator=(const SearchNode& p_rhs);

private:

    std::set<void*> m_storedData;
    SearchNode* m_next[256];
    SearchNode* m_failureNode;
};

#endif

