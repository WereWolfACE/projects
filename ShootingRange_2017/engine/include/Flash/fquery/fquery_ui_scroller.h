#pragma once

#include "fquery_ui.h"
#include <cmath>

namespace fquery{
	namespace ui{

		struct ScrollerParams{
			double min;
			double max;
			double step;

			double initial;

			double distanceToBreak;
			bool hardBreak;
		};

		

		FQuerySelector& scroller(FQuerySelector& selector, const ScrollerParams& params);
	}
}