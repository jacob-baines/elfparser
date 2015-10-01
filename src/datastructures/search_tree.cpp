#include "search_tree.hpp"
#include <boost/concept_check.hpp>

SearchTree::SearchTree() :
    m_rootNode(),
    m_ready(false),
    m_nodeVector()
{
}

SearchTree::~SearchTree()
{
    m_nodeVector.release();
}

void SearchTree::addWord(const std::string& p_string, void* p_storeData)
{
    addWord(reinterpret_cast<const unsigned char*>(p_string.c_str()),
               p_string.length(), p_storeData);
}

void SearchTree::addWord(const char* p_input, unsigned char p_length, void* p_storeData)
{
    addWord(reinterpret_cast<const unsigned char*>(p_input), p_length, p_storeData);
}

void SearchTree::addWord(const unsigned char* p_input,
                         unsigned char p_length, void* p_storeData)
{
    assert(!m_ready);
    doAddWord(&m_rootNode, p_input, p_length, p_storeData);
}

std::set<void*> SearchTree::search(const char* const p_inputString,
                                          const std::size_t p_inputLength)
{
    return search(reinterpret_cast<const unsigned char* const>(p_inputString),
                  p_inputLength);
}

std::set<void*> SearchTree::search(const unsigned char* const p_inputString,
                                          const std::size_t p_inputLength)
{
    assert(m_ready && p_inputString != NULL);

    std::set<void*> returnValue;
    SearchNode* currentNode = &m_rootNode;
    const unsigned char* end = p_inputString + p_inputLength;
    for (const unsigned char* start = p_inputString; start < end; ++start)
    {
        currentNode = currentNode->getNext(*start);
        if (!currentNode->getStoredData().empty())
        {
            const std::set<void*>& mergeThis(currentNode->getStoredData());
            returnValue.insert(mergeThis.begin(), mergeThis.end());
        }
    }

    return returnValue;
}

void SearchTree::compile()
{
    assert(!m_ready);

    // at the top level assign root as the failure node
    std::queue<SearchNode*> nodesByLevel;
    for (std::size_t i = 0; i < m_rootNode.getNextSize(); ++i)
    {
        SearchNode* next = m_rootNode.getNext(static_cast<const unsigned char>(i));
        if (!next)
        {
            m_rootNode.setNext(static_cast<const unsigned char>(i), &m_rootNode);
        }
        else
        {
            next->setFailure(&m_rootNode);
            nodesByLevel.push(next);
        }
    }

    // now loop through all levels computing failure nodes. Push more on as needed
    while (!nodesByLevel.empty())
    {
        SearchNode* currentNode = nodesByLevel.front();
        for (std::size_t i = 0; i < currentNode->getNextSize(); ++i)
        {
            SearchNode* next = currentNode->getNext(static_cast<const unsigned char>(i));
            if (next)
            {
                nodesByLevel.push(next);
                next->setFailure(
                    currentNode->getFailure()->getNext(
                        static_cast<const unsigned char>(i)));
                if (!next->getFailure()->getStoredData().empty())
                {
                    next->addReturnValues(next->getFailure()->getStoredData());
                }
            }
            else
            {
                currentNode->setNext(static_cast<const unsigned char>(i),
                                     currentNode->getFailure()->getNext(
                                         static_cast<const unsigned char>(i)));
            }
        }
        nodesByLevel.pop();
    }

    m_ready = true;
}

void SearchTree::doAddWord(SearchNode* p_node, const unsigned char* p_input,
                           unsigned char p_length, void* p_return)
{
    if (p_node->getNext(p_input[0]) == NULL)
    {
        /* create a node to transition to - store the allocated node in a vector
         * for easy clean up */
        m_nodeVector.push_back(new SearchNode());
        SearchNode* newNode = &m_nodeVector.back();

        p_node->setNext(p_input[0], newNode);
        p_node = newNode;
    }
    else
    {
        p_node = p_node->getNext(p_input[0]);
    }

    if (p_length == 1)
    {
        p_node->addReturnValue(p_return);
    }
    else
    {
        doAddWord(p_node, ++p_input, --p_length, p_return);
    }
}

std::set<const char*> SearchTree::findOffsets(const char* const p_inputString,
                                              const std::size_t p_inputLength)
{
    std::set<const char*> returnValue;
    SearchNode* currentNode = &m_rootNode;
    const char* end = p_inputString + p_inputLength;
    for (const char* start = p_inputString; start < end; ++start)
    {
        currentNode = currentNode->getNext(*start);
        if (!currentNode->getStoredData().empty())
        {
            returnValue.insert(start - 3);
        }
    }
    return returnValue;
}
