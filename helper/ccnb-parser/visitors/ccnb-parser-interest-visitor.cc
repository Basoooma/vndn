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

#include "ccnb-parser-interest-visitor.h"

#include "../syntax-tree/ccnb-parser-block.h"
#include "../syntax-tree/ccnb-parser-dtag.h"

#include "network/ndn-name-components.h"

#include "corelib/assert.h"

#include "network/ndn-interest-header.h"
#include "ccnb-parser-name-components-visitor.h"
#include "ccnb-parser-non-negative-integer-visitor.h"
#include "ccnb-parser-timestamp-visitor.h"
#include "ccnb-parser-nonce-visitor.h"

#include <boost/foreach.hpp>

#include "corelib/log.h"

NS_LOG_COMPONENT_DEFINE ("CcnbParserInterestVisitor");

namespace vndn
{
namespace CcnbParser
{

// We don't care about any other fields
void
InterestVisitor::visit (Dtag &n, boost::any param/*should be InterestHeader* */)
{
    // uint32_t n.m_dtag;
    // std::list<Ptr<Block> > n.m_nestedBlocks;

    static NonNegativeIntegerVisitor nonNegativeIntegerVisitor;
    static NameComponentsVisitor     nameComponentsVisitor;
    static TimestampVisitor          timestampVisitor;
    static NonceVisitor              nonceVisitor;

    InterestHeader &interest = *(boost::any_cast<InterestHeader *> (param));

    switch (n.m_dtag) {
    case CCN_DTAG_Interest:
        NS_LOG_DEBUG ("Interest");

        // process nested blocks
        BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags) {
            block->accept (*this, param);
        }
        break;
    case CCN_DTAG_Name: {
        NS_LOG_DEBUG ("Name");

        // process name components
        Ptr<NameComponents> name = Create<NameComponents> ();

        BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags) {
            block->accept (nameComponentsVisitor, &(*name));
        }
        interest.SetName (name);
        break;
    }
    case CCN_DTAG_MinSuffixComponents:
        NS_LOG_DEBUG ("MinSuffixComponents");
        if (n.m_nestedTags.size() != 1) // should be exactly one UDATA inside this tag
            throw CcnbDecodingException ();
        interest.SetMinSuffixComponents (
            boost::any_cast<uint32_t> (
                (*n.m_nestedTags.begin())->accept(
                    nonNegativeIntegerVisitor
                )));
        break;
    case CCN_DTAG_MaxSuffixComponents:
        NS_LOG_DEBUG ("MaxSuffixComponents");
        if (n.m_nestedTags.size() != 1) // should be exactly one UDATA inside this tag
            throw CcnbDecodingException ();
        interest.SetMaxSuffixComponents (
            boost::any_cast<uint32_t> (
                (*n.m_nestedTags.begin())->accept(
                    nonNegativeIntegerVisitor
                )));
        break;
    case CCN_DTAG_Exclude: {
        NS_LOG_DEBUG ("Exclude");
        // process exclude components
        Ptr<NameComponents> exclude = Create<NameComponents> ();

        BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags) {
            block->accept (nameComponentsVisitor, &(*exclude));
        }
        interest.SetExclude (exclude);
        break;
    }
    case CCN_DTAG_ChildSelector:
        NS_LOG_DEBUG ("ChildSelector");
        if (n.m_nestedTags.size() != 1) // should be exactly one UDATA inside this tag
            throw CcnbDecodingException ();

        interest.SetChildSelector (
            1 == boost::any_cast<uint32_t> (
                (*n.m_nestedTags.begin())->accept(
                    nonNegativeIntegerVisitor
                )));
        break;
    case CCN_DTAG_AnswerOriginKind:
        NS_LOG_DEBUG ("AnswerOriginKind");
        if (n.m_nestedTags.size() != 1) // should be exactly one UDATA inside this tag
            throw CcnbDecodingException ();
        interest.SetAnswerOriginKind (
            1 == boost::any_cast<uint32_t> (
                (*n.m_nestedTags.begin())->accept(
                    nonNegativeIntegerVisitor
                )));
        break;
    case CCN_DTAG_Scope:
        NS_LOG_DEBUG ("Scope");
        if (n.m_nestedTags.size() != 1) // should be exactly one UDATA inside this tag
            throw CcnbDecodingException ();
        interest.SetScope (
            boost::any_cast<uint32_t> (
                (*n.m_nestedTags.begin())->accept(
                    nonNegativeIntegerVisitor
                )));
        break;
    case CCN_DTAG_InterestLifetime:
        NS_LOG_DEBUG ("InterestLifetime");
        if (n.m_nestedTags.size() != 1) // should be exactly one UDATA inside this tag
            throw CcnbDecodingException ();

        interest.SetInterestLifetime (
            boost::any_cast<time_duration> (
                (*n.m_nestedTags.begin())->accept(
                    timestampVisitor
                )));
        break;
    case CCN_DTAG_Nonce:
        NS_LOG_DEBUG ("Nonce");
        if (n.m_nestedTags.size() != 1) // should be exactly one UDATA inside this tag
            throw CcnbDecodingException ();

        interest.SetNonce (
            boost::any_cast<uint32_t> (
                (*n.m_nestedTags.begin())->accept(
                    nonceVisitor
                )));
        break;


    case CCN_DTAG_Nack:
        NS_LOG_DEBUG ("Nack");
        if (n.m_nestedTags.size() != 1) // should be exactly one UDATA inside this tag
            throw CcnbDecodingException ();

        interest.SetNack (
            boost::any_cast<uint32_t> (
                (*n.m_nestedTags.begin())->accept(nonNegativeIntegerVisitor)));
        break;
    }
}

} // namespace CcnbParser
} // namespace vndn
