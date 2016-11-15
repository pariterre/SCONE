#include "CompositeMeasure.h"
#include "scone/sim/Factories.h"
#include "scone/core/Profiler.h"

namespace scone
{
	namespace cs
	{
		CompositeMeasure::Term::Term() :
		measure( nullptr )
		{
		}

		CompositeMeasure::Term::Term( const PropNode& pn ) :
		measure( nullptr ) // should be initialized by CompositeMeasure
		{
			INIT_PROPERTY_REQUIRED( pn, name );
			INIT_PROPERTY_REQUIRED( pn, weight );
			INIT_PROPERTY( pn, threshold, 0.0 );
			INIT_PROPERTY( pn, offset, 0.0 );
		}

		CompositeMeasure::Term::Term( Term&& other ) :
		name( other.name ),
		weight( other.weight ),
		threshold( other.threshold ),
		offset( other.offset ),
		measure( std::move( other.measure ) )
		{
		}

		CompositeMeasure::CompositeMeasure( const PropNode& props, opt::ParamSet& par, sim::Model& model, const sim::Area& area ) :
		Measure( props, par, model, area )
		{
			// get Terms (obsolete)
			if ( const PropNode* termNode = props.try_get_child( "Terms" ) )
			{
				for ( auto it = termNode->begin(); it != termNode->end(); ++it )
				{
					Term t( it->second );

					// cast a ControllerUP to a Measure* using release(), because we don't have a CreateMeasure() factory
					Measure* m = dynamic_cast<Measure*>( sim::CreateController( it->second.get_child( "Measure" ), par, model, area ).release() );
					SCONE_THROW_IF( m == nullptr, "Could not cast Controller* to Measure*" );
					t.measure = MeasureUP( m );
					m_Terms.push_back( std::move( t ) ); // use std::move because Term has a unique_ptr member
				}
			}

			// get Measures
			if ( const PropNode* mprops = props.try_get_child( "Measures" ) )
			{
				for ( auto it = mprops->begin(); it != mprops->end(); ++it )
				{
					// cast a ControllerUP to a Measure* using release(), because we don't have a CreateMeasure() factory
					Measure* m = dynamic_cast<Measure*>( sim::CreateController( it->second, par, model, area ).release() );
					SCONE_THROW_IF( m == nullptr, "Could not cast Controller* to Measure*" );
					m_Measures.push_back( MeasureUP( m ) );
				}
			}
		}

		void CompositeMeasure::StoreData( Storage< Real >::Frame& frame )
		{
			for ( Term& t: m_Terms )
				t.measure->StoreData( frame );

			for ( MeasureUP& m: m_Measures )
				m->StoreData( frame );
		}

		CompositeMeasure::~CompositeMeasure() { }

		sim::Controller::UpdateResult CompositeMeasure::UpdateAnalysis( const sim::Model& model, double timestamp )
		{
			SCONE_PROFILE_SCOPE;

			if ( !IsActive( model, timestamp ) )
				return NoUpdate;

			bool terminate = false;

			// update Terms (obsolete)
			for ( Term& t: m_Terms )
				terminate |= t.measure->UpdateAnalysis( model, timestamp ) == RequestTermination;

			// update Measures
			for ( MeasureUP& m: m_Measures )
				terminate |= m->UpdateAnalysis( model, timestamp ) == RequestTermination;

			return terminate ? RequestTermination : SuccessfulUpdate;
		}

		double CompositeMeasure::GetResult( sim::Model& model )
		{
			double total = 0.0;
			for ( Term& t: m_Terms )
			{
				double org_result = t.measure->GetResult( model );
				double ofset_result = org_result + t.offset;
				double thresh_result = ofset_result <= t.threshold ? 0.0 : ofset_result;
				double weighted_result = t.weight * thresh_result;

				total += weighted_result;

				GetReport().add_child( t.name, t.measure->GetReport() ).set( stringf( "%g\t%g * (%g + %g if > %g)", weighted_result, t.weight, org_result, t.offset, t.threshold ) );
			}

			for ( MeasureUP& m: m_Measures )
			{
				double org_result = m->GetResult( model );
				double ofset_result = org_result + m->GetOffset();
				double thresh_result = ofset_result <= m->GetThreshold() ? 0.0 : ofset_result;
				double weighted_result = m->GetWeight() * thresh_result;

				total += weighted_result;

				GetReport().add_child( m->GetName(), m->GetReport() ).set( stringf( "%g\t%g * (%g + %g if > %g)", weighted_result, m->GetWeight(), org_result, m->GetOffset(), m->GetThreshold() ) );
			}

			GetReport().set( total );

			return total;
		}

		scone::String CompositeMeasure::GetClassSignature() const
		{
			String str;
			for ( auto& t: m_Terms )
				str += t.measure->GetSignature();

			for ( auto& m: m_Measures )
				str += m->GetSignature();

			return str;
		}
	}
}
