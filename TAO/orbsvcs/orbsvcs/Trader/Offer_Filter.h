/* -*- C++ -*- */

// ========================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Offer_Filter.h
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_OFFER_FILTER_H
#define TAO_OFFER_FILTER_H

#include "CosTradingC.h"
#include "Policies.h"
#include "Property_Evaluator.h"

#include <set>
#include <string>

class TAO_Offer_Filter
// = TITLE
//    The purpose of this class is to ensure that offers that
//    shouldn't be considered by the TAO_Constraint_Interpreter
//    aren't.
// 
// = DESCRIPTION
//    There two classes of reasons why an offer for a correct
//    type shouldn't be considered: 1) The default parameters of the
//    Trader or policies passed to the Lookup::query method deem it
//    inappropriate to consider offers with modifiable (i.e., not
//    readonly) or dynamic properties. 2) We've exceeded the
//    default or provided cardinality constraints. TAO_Offer_Filter
//    ensures that violation of policies doesn't occur. It's the
//    enforcer. 
{
public:
  
  typedef CosTradingRepos::ServiceTypeRepository SERVICE_TYPE_REPOS;
  
  TAO_Offer_Filter (SERVICE_TYPE_REPOS::TypeStruct* type_struct,
		    TAO_Policies& policies);
  // Glean from the TypeStruct and Policy setting the appropriate way
  // to screen unsuitable offers from consideration.
  
  CORBA::Boolean ok_to_consider (CosTrading::Offer* offer);
  // Determine whether the poicies contained in the given policy
  // object allow the Lookup interface to consider the offer. That is, 
  // if use_modifiable_properties is false, and the offer contains
  // modifiable properties as designated in the type struct, return
  // false. If use_dynamic_properties is false, and the offer contains 
  // dynamic properties, then return false. If the lookup interface is 
  // safe in considering this offer, return true and subtract from the 
  // search card value. When the search card value falls to zero,
  // ok_to_consider always returns false.
  
  CORBA::Boolean ok_to_consider_more (void);
  // It's ok to consider more offers when lookup hasn't exceeded the
  // cardinality values for searching and matching offers.

  void matched_offer (void);
  // Signal that the Lookup method has matched an offer; decrement the 
  // match_card.
  
private:

  typedef set<string, less<string> > PROP_NAMES;

  PROP_NAMES mod_props_;
  // The set of the name of modifiable properties.
  
  CORBA::ULong search_card_, match_card_;
  // Keep track of the cardinalities.
  
  CORBA::Boolean dp_;
  CORBA::Boolean mod_;
  // Keep track of property limitations: modifiable or dynamic ones
  // may be bad.
};

#endif /* TAO_OFFER_FILTER_H */
