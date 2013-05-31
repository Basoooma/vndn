/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ccnb-parser-void-depth-first-visitor.h"

#include "../syntax-tree/ccnb-parser-blob.h"
#include "../syntax-tree/ccnb-parser-udata.h"
#include "../syntax-tree/ccnb-parser-tag.h"
#include "../syntax-tree/ccnb-parser-dtag.h"
#include "../syntax-tree/ccnb-parser-attr.h"
#include "../syntax-tree/ccnb-parser-dattr.h"
#include "../syntax-tree/ccnb-parser-ext.h"

#include <boost/foreach.hpp>

namespace vndn
{
namespace CcnbParser
{

void
VoidDepthFirstVisitor::visit (Blob &n, boost::any param)
{
    // Buffer n.m_blob;
}

void
VoidDepthFirstVisitor::visit (Udata &n, boost::any param)
{
    // std::string n.m_udata;
}

void
VoidDepthFirstVisitor::visit (Tag &n, boost::any param)
{
    // std::string n.m_tag;
    // std::list<Ptr<Block> > n.m_attrs;
    // std::list<Ptr<Block> > n.m_nestedBlocks;
    BOOST_FOREACH (Ptr<Block> block, n.m_attrs) {
        block->accept (*this, param);
    }
    BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags) {
        block->accept (*this, param);
    }
}

void
VoidDepthFirstVisitor::visit (Dtag &n, boost::any param)
{
    // std::string n.m_tag;
    // std::list<Ptr<Block> > n.m_attrs;
    // std::list<Ptr<Block> > n.m_nestedBlocks;
    BOOST_FOREACH (Ptr<Block> block, n.m_attrs) {
        block->accept (*this, param);
    }
    BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags) {
        block->accept (*this, param);
    }
}

void
VoidDepthFirstVisitor::visit (Attr &n, boost::any param)
{
    // std::string n.m_attr;
    // Ptr<Udata> n.m_value;
}

void
VoidDepthFirstVisitor::visit (Dattr &n, boost::any param)
{
    // uint32_t n.m_dattr;
    // Ptr<Udata> n.m_value;
}

void
VoidDepthFirstVisitor::visit (Ext &n, boost::any param)
{
    // uint64_t n.m_extSubtype;
}

}
}
