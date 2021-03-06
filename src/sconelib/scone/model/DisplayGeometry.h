#pragma once

#include "scone/core/Vec3.h"
#include "scone/core/Quat.h"
#include "scone/core/types.h"
#include "xo/filesystem/path.h"

namespace scone
{
	struct DisplayGeometry
	{
		Vec3 pos;
		Quat ori;
		Vec3 scale;
		xo::path filename;
	};
}
