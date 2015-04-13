#ifndef VIEW_H
#define VIEW_H

#include <ostream>
#include <fstream>
#include <vector>
#include "ParserDom.h"
#include "Node.h"


namespace htmlcxx {
    namespace HTML {

typedef tree<Node> treeNode;
typedef treeNode::iterator tag;
typedef std::vector< treeNode > vecTreeNode;
typedef std::vector<treeNode::iterator> vecItrNode;

class Page
{
    HTML::ParserDom html;
    std::map<std::string, treeNode::iterator> mapIdItrNode;
    std::map<std::string, vecItrNode> mapNameItrNodes;
public:
    Page();
    void setContent(const std::ifstream& file);
    void setContent(const std::string& html);
    void insertContentId(std::string id, Page& view);
    void insertContentId(std::string id, const std::ifstream& file);
    void insertContentId(std::string id, const std::string& html);
    void setContentId(std::string id, const std::string& html);
    void setContentId(std::string id, const std::ifstream& file);
    void write(std::ostream& out, const treeNode& dom);
    treeNode& getDom();
    treeNode::iterator getTagById(std::string id);
    vecItrNode getTagsByName(std::string name );
    void setText(treeNode::iterator node, const std::string& text);
private:
    void insertMapNameNode(treeNode::iterator& it);
    void mapeiaNameNode();
    void mapeiaIdNode();
};


    }
}

std::ostream& operator<<(std::ostream &stream, htmlcxx::HTML::Page &view);

#endif // VIEW_H
