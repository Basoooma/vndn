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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

///< #CCN_PR_SCOPE0 (0x20) local scope,
///< #CCN_PR_SCOPE1 (0x40) this host,
///< #CCN_PR_SCOPE2 (0x80) immediate neighborhood

#include "ndn-interest-header.h"

#include "corelib/log.h"
#include "helper/ndn-encoding-helper.h"
#include "helper/ndn-decoding-helper.h"

NS_LOG_COMPONENT_DEFINE ("InterestHeader");

namespace vndn
{

InterestHeader::InterestHeader ()
    : m_minSuffixComponents (-1)
    , m_maxSuffixComponents (-1)
    , m_childSelector (false)
    , m_answerOriginKind (false)
    , m_scope (-1)
    , m_interestLifetime (seconds(0))
    , m_nonce (0)
    , m_nackType (NORMAL_INTEREST)
{
}

void InterestHeader::SetName (const Ptr<NameComponents> &name)
{
    m_name = name;
}

Ptr<const NameComponents> InterestHeader::GetName () const
{
    if (m_name == 0) throw InterestHeaderException();
    return m_name;
}

void InterestHeader::SetMinSuffixComponents (int32_t value)
{
    m_minSuffixComponents = value;
}

int32_t InterestHeader::GetMinSuffixComponents () const
{
    return m_minSuffixComponents;
}

void InterestHeader::SetMaxSuffixComponents (int32_t value)
{
    m_maxSuffixComponents = value;
}

int32_t InterestHeader::GetMaxSuffixComponents () const
{
    return m_maxSuffixComponents;
}

void InterestHeader::SetExclude (const Ptr<NameComponents> &exclude)
{
    m_exclude = exclude;
}

bool InterestHeader::IsEnabledExclude () const
{
    return m_exclude != 0;
}

const NameComponents &InterestHeader::GetExclude () const
{
    if (m_exclude == 0) throw InterestHeaderException();
    return *m_exclude;
}

void InterestHeader::SetChildSelector (bool value)
{
    m_childSelector = value;
}

bool InterestHeader::IsEnabledChildSelector () const
{
    return m_childSelector;
}

void InterestHeader::SetAnswerOriginKind (bool value)
{
    m_answerOriginKind = value;
}

bool InterestHeader::IsEnabledAnswerOriginKind () const
{
    return m_answerOriginKind;
}

void InterestHeader::SetScope (int8_t scope)
{
    m_scope = scope;
}

int8_t InterestHeader::GetScope () const
{
    return m_scope;
}

void InterestHeader::SetInterestLifetime (const time_duration &lifetime)
{
    m_interestLifetime = lifetime;
}

time_duration InterestHeader::GetInterestLifetime () const
{
    return m_interestLifetime;
}

void InterestHeader::SetNonce (uint32_t nonce)
{
    m_nonce = nonce;
}

uint32_t InterestHeader::GetNonce () const
{
    return m_nonce;
}

void InterestHeader::SetNack (uint32_t nackType)
{
    m_nackType = nackType;
}

uint32_t InterestHeader::GetNack () const
{
    return m_nackType;
}

uint32_t InterestHeader::GetSize (void) const
{
    // unfortunately, we don't know exact header size in advance
    return NDNEncodingHelper::GetSize (*this);
}

void InterestHeader::Serialize(Buffer &buffer) const
{
    NDNEncodingHelper::Serialize(buffer, *this);
}

uint32_t InterestHeader::Deserialize (const Buffer &buffer)
{
    return NDNDecodingHelper::Deserialize (buffer, *this); // \todo Debugging is necessary
}

void InterestHeader::Print (std::ostream &os) const
{
    os << "<Interest>\n  <Name>" << *GetName() << "</Name>\n";
    if (GetNack() > 0) {
        os << "  <NACK>";
        switch (GetNack ()) {
        case NACK_LOOP:
            os << "loop";
            break;
        case NACK_CONGESTION:
            os << "congestion";
            break;
        default:
            os << "unknown";
            break;
        }
        os << "</NACK>\n";
    }
    if (GetMinSuffixComponents() >= 0)
        os << "  <MinSuffixComponents>" << GetMinSuffixComponents() << "</MinSuffixComponents>\n";
    if (GetMaxSuffixComponents() >= 0)
        os << "  <MaxSuffixComponents>" << GetMaxSuffixComponents() << "</MaxSuffixComponents>\n";
    if (IsEnabledExclude() && GetExclude().size() > 0)
        os << "  <Exclude>" << GetExclude() << "</Exclude>\n";
    if (IsEnabledChildSelector())
        os << "  <ChildSelector />\n";
    if (IsEnabledAnswerOriginKind())
        os << "  <AnswerOriginKind />\n";
    if (GetScope() >= 0)
        os << "  <Scope>" << GetScope() << "</Scope>\n";
    if (GetInterestLifetime() != seconds(0))
        os << "  <InterestLifetime>" << GetInterestLifetime() << "</InterestLifetime>\n";
    if (GetNonce() > 0)
        os << "  <Nonce>" << GetNonce () << "</Nonce>\n";
    os << "</Interest>";
}

}
