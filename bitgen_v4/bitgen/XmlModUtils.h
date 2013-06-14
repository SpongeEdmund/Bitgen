//! This is used to create an XML model.
/*!
	

	If the node library is described in the XML like this:

	<library name="fd" location="shanghai">
	  <book isbn="xxxx-xxxx-xxxx" name="1984" amount="1"/>
	  <book isbn="yyyy-yyyy-yyyy" name="2666" amount="2"/>
	</library>

	It can be modeled as follows:

	DEFINE_NODE(book)
	{
		HAS_ATTRIBUTE(isbn)
		HAS_ATTRIBUTE(name)
		HAS_ATTRIBUTE(amount)
	};

	DEFINE_NODE(library)
	{
		HAS_ATTRIBUTE(name)
		HAS_ATTRIBUTE(location)
		HAS_CHILDREN_FOUND_BY1(book, isbn) // Every kind of book has its own ISBN, so ISBN can be used as an index
	};

	You should be aware that the query results using this modeling method are always string type.
	Before you try to use them, lexical cast them to the real type you need.
*/

#ifndef __XML_MOD_UTILS_H__
#define __XML_MOD_UTILS_H__

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"

#include <string>
#include <iostream>

using namespace rapidxml;
using std::string;

#define DEFINE_NODE( node ) struct node : public xml_node<>

#define HAS_ATTRIBUTE( attr ) \
	const string get_##attr() const { return first_attribute(#attr)->value(); }

#define HAS_HIDDEN_ATTRIBUTE( attr, default_value ) \
	const string get_##attr() const { \
	if ( first_attribute(#attr) ) \
		return first_attribute(#attr)->value(); \
	else \
		return default_value; \
	}

#define HAS_CHILDREN( node ) \
	node* first_##node() { return static_cast<node*>( first_node(#node) ); }

#define HAS_SIBLING( node ) \
	node* next_##node() { return static_cast<node*>( next_sibling(#node) ); }

#define HAS_CHILDREN_FOUND_BY1( node, index ) \
	node* find_##node##_by_##index( const string & to_find ) { \
		xml_node<>* n = first_node(#node); \
		for (  ; n; n = n->next_sibling(#node) ) { \
			node* cur_node = static_cast<node*>(n); \
			if ( to_find == cur_node->get_##index() ) \
				return cur_node;\
		}\
		return 0;\
	}

#define HAS_CHILDREN_FOUND_BY2( node, index1, index2 ) \
	node* find_##node##_by_##index1##_and_##index2( const string & to_find1, const string & to_find2 ) { \
		xml_node<>* n = first_node(#node); \
		for (  ; n; n = n->next_sibling(#node) ) { \
			node* cur_node = static_cast<node*>(n); \
			if ( to_find1 == cur_node->get_##index1() && to_find2 == cur_node->get_##index2() ) \
				return cur_node;\
		}\
		return 0;\
	}

#define HAS_ONE_CHILD( node ) \
	node* get_##node() {\
		return static_cast<node*>( first_node(#node) );\
	}






#endif