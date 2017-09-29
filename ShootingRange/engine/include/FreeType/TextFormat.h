#pragma once

#include "FreeTypeText.h"
#include "TextStyle.h"

namespace freetype
{
	void setSourceDecimalPoint( const std::string &srcDecimalPoint);
	void setGroupThousands( bool groupThousands);
	void setThousandsSeparator( const std::string &thousandsSeparator);
	void setDecimalPoint( const std::string &decimalPoint);
}
