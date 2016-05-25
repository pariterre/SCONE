#include "MetaReflexParams.h"

namespace scone
{
	namespace cs
	{
		MetaReflexParams::MetaReflexParams( const PropNode& props, opt::ParamSet& par, sim::Model& model, const String& pnp )
		{
			Init( props, par, model, pnp );
		}

		void MetaReflexParams::Init( const PropNode& props, opt::ParamSet& par, sim::Model& model, const String& pnp )
		{
			const PropNode& mrcprops = model.GetCustomProps().TryGetChild( "meta_reflex_control" );

			ref_pos = mrcprops.Get( "use_ref", true ) ? Degree( par.TryGet( "R" + pnp, props, "R" + pnp, 0.0 ) ) : Degree( 0 );
			length_gain = mrcprops.Get( "use_length", true ) ? par.TryGet( "L" + pnp, props, "L" + pnp, 0.0 ) : 0;
			force_gain = mrcprops.Get( "use_force", true ) ? par.TryGet( "F" + pnp, props, "F" + pnp, 0.0 ) : 0;
			constant = mrcprops.Get( "use_constant", true ) ? par.TryGet( "C" + pnp, props, "C" + pnp, 0.0 ) : 0;
			stiffness = mrcprops.Get( "use_stiffness", true ) ? par.TryGet( "S" + pnp, props, "S" + pnp, 0.0 ) : 0;
			symmetry = mrcprops.Get( "use_symmetry", true ) ? par.TryGet( "sym" + pnp, props, "sym" + pnp, 0.0 ) : 0;
		}
	}
}