/*
** ReactionForceMeasure.cpp
**
** Copyright (C) 2013-2019 Thomas Geijtenbeek and contributors. All rights reserved.
**
** This file is part of SCONE. For more information, see http://scone.software.
*/

#include "ReactionForceMeasure.h"

#include "scone/core/HasName.h"
#include "scone/model/Model.h"

namespace scone
{
	ReactionForceMeasure::ReactionForceMeasure( const PropNode& props, Params& par, Model& model, const Location& loc ) :
	Measure( props, par, model, loc ),
	RangePenalty< Real >( props )
	{}

	double ReactionForceMeasure::ComputeResult( Model& model )
	{
		return RangePenalty<Real>::GetResult();
	}

	bool ReactionForceMeasure::UpdateMeasure( const Model& model, double timestamp )
	{
		Real leg_load = 0.0f;
		for ( auto& leg : model.GetLegs() )
			leg_load += leg->GetContactForce().length() / model.GetBW();

		AddSample( timestamp, leg_load );

		return false;
	}

	void ReactionForceMeasure::StoreData( Storage< Real >::Frame& frame, const StoreDataFlags& flags ) const
	{
		// TODO: store joint load value
		frame[ "legs.load_penalty" ] = GetLatest();
	}
}
