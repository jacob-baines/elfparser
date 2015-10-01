#ifndef ELFPARSER_SEARCH_HPP
#define ELFPARSER_SEARCH_HPP

#include "search_node.hpp"

#include <set>
#include <queue>
#include <string>

#include <boost/ptr_container/ptr_vector.hpp>

/*
 *  This is a basic automata implemenation. Used for efficient searching.
 */
class SearchTree
{
public:

    SearchTree();
    ~SearchTree();

    void addWord(const unsigned char* p_input,
                    unsigned char p_length, void* p_storeData);
    void addWord(const char* p_input, unsigned char p_length, void* p_storeData);
    void addWord(const std::string& p_string,
                    void* p_storeData);

    std::set<const char*> findOffsets(const char* const p_inputString,
                                      const std::size_t p_inputLength);
    std::set<void*> search(const char* const p_inputString,
                           const std::size_t p_inputLength);

    std::set<void*> search(const unsigned char* const p_inputString,
                                  const std::size_t p_inputLength);

    void compile();

private:

    void doAddWord(SearchNode* p_node, const unsigned char* p_input,
                        unsigned char p_length, void* p_storeData);

private:

    SearchNode m_rootNode;

    bool m_ready;

    boost::ptr_vector<SearchNode> m_nodeVector;
};

#endif

