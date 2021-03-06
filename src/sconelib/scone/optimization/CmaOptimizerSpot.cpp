/*
** CmaOptimizerSpot.cpp
**
** Copyright (C) 2013-2019 Thomas Geijtenbeek and contributors. All rights reserved.
**
** This file is part of SCONE. For more information, see http://scone.software.
*/

#include "CmaOptimizerSpot.h"

#include <random>
#include "spot/stop_condition.h"
#include "spot/file_reporter.h"
#include "spot/console_reporter.h"

#include "scone/core/Exception.h"
#include "scone/core/Log.h"

namespace scone
{
	using std::cout;
	using std::endl;

	CmaOptimizerSpot::CmaOptimizerSpot( const PropNode& pn, const PropNode& pn_root ) :
	CmaOptimizer( pn, pn_root ),
	cma_optimizer( *m_Objective, lambda_, CmaOptimizer::random_seed )
	{
		size_t dim = GetObjective().dim();
		SCONE_ASSERT( dim > 0 );

		lambda_ = lambda();
		mu_ = mu();
		sigma_ = sigma();
		set_max_threads( ( int )max_threads );
		enable_fitness_tracking( window_size );

	}

	void CmaOptimizerSpot::SetOutputMode( OutputMode m )
	{
		xo_assert( output_mode_ == no_output ); // output mode can only be set once
		output_mode_ = m;
		switch ( output_mode_ )
		{
		case scone::Optimizer::no_output:
			break;
		case scone::Optimizer::console_output:
			add_new_reporter< spot::console_reporter >();
			break;
		case scone::Optimizer::status_output:
			add_new_reporter< CmaOptimizerReporter >();
			break;
		default: SCONE_THROW( "Unknown output mode" );
		}
	}

	void CmaOptimizerSpot::Run()
	{
		// create output folder
		PrepareOutputFolder();

		// reporters
		auto rep = add_new_reporter< spot::file_reporter >( GetOutputFolder() );
		rep->min_improvement_for_file_output = min_improvement_for_file_output;
		rep->max_steps_without_file_output = max_generations_without_file_output;

		// stop conditions
		add_new_stop_condition< spot::max_steps_condition >( max_generations );
		add_new_stop_condition< spot::min_progress_condition >( min_progress, min_progress_samples );
		find_stop_condition< spot::flat_fitness_condition >().epsilon_ = flat_fitness_epsilon_;

		run();
	}

	void CmaOptimizerReporter::on_start( const optimizer& opt )
	{
		auto& cma = dynamic_cast< const CmaOptimizerSpot& >( opt );

		log::info( "Starting optimization ", cma.id(), " dim=", cma.dim(), " lambda=", cma.lambda(), " mu=", cma.mu() );
		if ( cma.GetStatusOutput() )
		{
			PropNode pn = cma.GetStatusPropNode();
			pn.set( "folder", cma.GetOutputFolder() );
			pn.set( "dim", cma.dim() );
			pn.set( "sigma", cma.sigma() );
			pn.set( "lambda", cma.lambda() );
			pn.set( "mu", cma.mu() );
			pn.set( "max_generations", cma.max_generations );
			pn.set( "minimize", cma.IsMinimizing() );
			pn.set( "window_size", cma.window_size );
			cma.OutputStatus( pn );
		}
	}

	void CmaOptimizerReporter::on_stop( const optimizer& opt, const spot::stop_condition& s )
	{
		auto& cma = dynamic_cast< const CmaOptimizerSpot& >( opt );
		cma.OutputStatus( "finished", s.what() );
		log::info( "Optimization ", cma.id(), " finished: ", s.what() );
	}

	void CmaOptimizerReporter::on_pre_evaluate_population( const optimizer& opt, const search_point_vec& pop )
	{
		auto& cma = dynamic_cast< const CmaOptimizerSpot& >( opt );
	}

	void CmaOptimizerReporter::on_post_evaluate_population( const optimizer& opt, const search_point_vec& pop, const fitness_vec_t& fitnesses, bool new_best )
	{
		auto& cma = dynamic_cast< const CmaOptimizerSpot& >( opt );

		// report results
		auto pn = cma.GetStatusPropNode();
		pn.set( "step", cma.current_step() );
		pn.set( "step_best", cma.current_step_best_fitness() );
		pn.set( "step_median", xo::median( cma.current_step_fitnesses() ) );
		pn.set( "trend_offset", cma.fitness_trend().offset() );
		pn.set( "trend_slope", cma.fitness_trend().slope() );
		if ( new_best )
		{
			pn.set( "best", cma.best_fitness() );
			pn.set( "best_gen", cma.current_step() );
		}

		cma.OutputStatus( pn );

		//cma.OutputStatus( "generation", xo::stringf( "%d %g %g %g %g %g", cma.current_step(), cma.current_step_best(), cma.current_step_median(), cma.current_step_average(), cma.fitness_trend().offset(), cma.fitness_trend().slope() ) );
		//if ( new_best )
		//	cma.OutputStatus( "best", cma.best_fitness() );
	}
}
