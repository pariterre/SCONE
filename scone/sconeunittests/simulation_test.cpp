#include "common.h"

#include "scone/cs/tools.h"

#include <boost/filesystem.hpp>
#include <boost/predef.h>
#include "platform.h"

using namespace boost::filesystem;
using std::cout;
using std::endl;

BOOST_AUTO_TEST_CASE( simulation_test )
{
	path testpath = scone::GetSconeFolder( "root" ) + "/unittestdata/simulation_test";

	for ( directory_iterator dir_it( testpath ); dir_it != directory_iterator(); ++dir_it )
	{
		if ( is_directory( dir_it->path() ) )
		{
			for ( directory_iterator fileit( dir_it->path() ); fileit != directory_iterator(); ++fileit )
			{
				//cout << "Checking file " << fileit->path().string() << endl;
				if ( fileit->path().extension() == ".par" )
				{
					path fp = fileit->path();
					// cout << "Running simulation for " << fp << endl;
					auto result = scone::RunSimulation( fp.string() );
					path reportpath = fp.parent_path() / ( "result_" + make_platform_id() + "_" + fp.stem().string() + ".xml" );
					if ( !exists( reportpath ) )
					{
						BOOST_ERROR( "Could not find simulation report: " + reportpath.string() );
						result.GetChild( "report" ).ToXmlFile( reportpath.string(), "report" );
					}
					else
					{
						auto verify = scone::ReadPropNodeFromXml( reportpath.string() );
						auto rep1 = result.GetChild( "report" );
						auto rep2 = verify.GetChild( "report" );
						BOOST_CHECK( rep1 == rep2 );
						if ( rep1 != rep2 )
						{
							BOOST_MESSAGE( "rep1:" );
							BOOST_MESSAGE( rep1 );
							BOOST_MESSAGE( "rep2:" );
							BOOST_MESSAGE( rep2 );
						}
					}
				}
			}
		}
	}
}