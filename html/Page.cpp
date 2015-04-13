#include "Page.h"
#include <fstream>
#include <sstream>

namespace htmlcxx {
    namespace HTML {

    using namespace std;

Page::Page()
{
}

void Page::setContent(const ifstream& file)
{
    stringstream ss;
    ss << file.rdbuf();
    setContent(ss.str());
}

void Page::setContent(const string &htmlText)
{
    html.parse(htmlText);
}

void Page::setContentId(string id, const string& html)
{
	HTML::ParserDom dom;
	dom.parse(html);
	const treeNode& newTree = (treeNode&)dom.getTree();

	treeNode::iterator it = getTagById(id);
	if (getDom().is_valid(it)){
		treeNode& tree = const_cast<treeNode&>(getDom());
		tree.erase_children(it);
		tree.append_child(it, newTree.begin());
	}

	mapeiaIdNode();
	mapeiaNameNode();
}

void Page::setContentId(string id, const ifstream& file)
{
    stringstream ss;
    ss << file.rdbuf();
    
	setContentId(id, ss.str());
}

void Page::insertContentId(string id, Page &view)
{
    treeNode::iterator it = getTagById(id);
    if(getDom().is_valid(it)){
        treeNode& tree = const_cast<treeNode&>(getDom());
        tree.append_children(it, view.getDom().begin(), view.getDom().end());
    }

    mapeiaIdNode();
    mapeiaNameNode();
}

void Page::insertContentId(string id, const ifstream& file)
{
    Page view;
    view.setContent(file);
    insertContentId(id, view);
}

void Page::insertContentId(string id, const string &html)
{
    Page view;
    view.setContent(html);
    insertContentId(id, view);
}

treeNode& Page::getDom()
{
    return const_cast<treeNode&>((const treeNode&)html.getTree());
}

treeNode::iterator Page::getTagById(string id)
{
    if(mapIdItrNode.empty())
        mapeiaIdNode();
    std::map<std::string, treeNode::iterator>::iterator it = mapIdItrNode.find(id);
    if(it!=mapIdItrNode.end())
        return it->second;

    return getDom().end();
}

void Page::write(std::ostream& out, const treeNode& dom)
{
    treeNode::iterator it = dom.begin();
    it->parseAttributes();

    const string& text = it->text();
    out << text;

    for ( unsigned i=0; i<dom.number_of_children(it); i++ )
    {
        write(out, dom.child(it,i) );
    }

    const string &closinText = it->closingText();
    out << closinText;
}

void Page::insertMapNameNode(treeNode::iterator& it)
{
    it->parseAttributes();
    const std::pair<bool, std::string>& attr = it->attribute("name");
    if(attr.first)
        mapNameItrNodes[attr.second].push_back(it);
}

void Page::mapeiaNameNode()
{
    mapNameItrNodes.clear();
    for (treeNode::iterator it = getDom().begin(); it != getDom().end(); it++)
    {
		if (it == getDom().end())
			return;
        insertMapNameNode(it);
    }
}

void Page::mapeiaIdNode()
{
    mapIdItrNode.clear();
    for(treeNode::iterator it=getDom().begin(); it!=getDom().end(); it++)
    {
        if(it->attributes().empty())
            it->parseAttributes();
        const std::pair<bool, std::string>& attr = it->attribute("id");
        if(attr.first)
            mapIdItrNode[attr.second] = it;
    }
}

vecItrNode Page::getTagsByName(string id)
{
    if(mapNameItrNodes.empty()){
        mapeiaNameNode();
    }

    std::map<std::string, vecItrNode>::iterator it = mapNameItrNodes.find(id);
    if(it != mapNameItrNodes.end())
        return it->second;

    return vecItrNode();
}

void Page::setText(treeNode::iterator node, const string &text)
{
    if(getDom().is_valid(node))
        for ( unsigned i=0; i<getDom().number_of_children(node); i++ )
        {
            treeNode::iterator it = getDom().child(node, i);
            if ( !it->isTag() && !it->isComment() )
            {
                it->text( text );
            }
        }
}

    }
} // end namespace

std::ostream& operator<<(std::ostream &stream, htmlcxx::HTML::Page &view)
{
    view.write(stream, view.getDom());
	return stream;
}
