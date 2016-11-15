#include "propnode_tools.h"

namespace scone
{
	String GetCleanVarName( const String& str )
	{
		return str.substr( 0, 2 ) == "m_" ? str.substr( 2 ) : str;
	}

	void LogUntouched( const PropNode& p, log::Level level, size_t depth )
	{
		if ( depth == 0 )
		{
			size_t unused = p.count_untouched();
			if ( unused > 0 )
			{
				log::WarningF( "Warning, %d unused parameters found:", unused );
			}
			else return; // do nothing
		}

		// find more touched
		for ( PropNode::const_iterator iter = p.GetChildren().begin(); iter != p.GetChildren().end(); ++iter )
		{
			// find if any child is touched (err...)
			if ( iter->second->count_untouched() > 0 )
			{
				std::stringstream str;
				str << String( depth * 2, ' ' ) << iter->first;

				if ( !iter->second->touched() )
				{
					if ( iter->second->HasValue() )
						str << " = " << iter->second->GetValueType();
					str << " *";
				}

				log::LogMessage( level, str.str( ) );
			}

			LogUntouched( *iter->second, level, depth + 1 );
		}
	}
}
