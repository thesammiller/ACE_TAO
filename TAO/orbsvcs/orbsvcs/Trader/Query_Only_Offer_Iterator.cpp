 /* -*- C++ -*- */

// ============================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Query_Only_Offer_Iterator.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ============================================================================
#define ACE_BUILD_DLL

#include "Query_Only_Offer_Iterator.h"
#include <iostream.h>

TAO_Query_Only_Offer_Iterator::
TAO_Query_Only_Offer_Iterator(const TAO_Property_Filter& pfilter)
  : TAO_Offer_Iterator (pfilter)
{
}

TAO_Query_Only_Offer_Iterator::~TAO_Query_Only_Offer_Iterator(void)
{
  //  cerr << "In iterator destructor now" << endl;
}

void
TAO_Query_Only_Offer_Iterator::add_offer (CosTrading::OfferId offer_id,
					  CosTrading::Offer* offer)
{
  this->offers_.push_back (offer);
  CORBA::string_free (offer_id);
}

CORBA::ULong 
TAO_Query_Only_Offer_Iterator::max_left (CORBA::Environment& _env) 
  TAO_THROW_SPEC(CORBA::SystemException, 
		 CosTrading::UnknownMaxLeft)
{
  return this->offers_.size ();
}

CORBA::Boolean 
TAO_Query_Only_Offer_Iterator::next_n (CORBA::ULong n, 
				       OfferSeq_out offers,
				       CORBA::Environment& _env) 
  TAO_THROW_SPEC (CORBA::SystemException)
{
  offers = new CosTrading::OfferSeq;

  CORBA::ULong offers_in_sequence =
    min (n, (CORBA::ULong)this->offers_.size ());
  offers->length (offers_in_sequence);
  
  // populate the sequence.
  for (CORBA::ULong i = 0; i < offers_in_sequence; i++)
    {
      CosTrading::Offer& source = *(this->offers_.front ());
      CosTrading::Offer& destination = (*offers)[i];

      this->pfilter_.filter_offer (source, destination);
      offers_.pop_front ();
    }
  
  return offers_in_sequence != 0;
}


