#pragma once

#include "Function.h"

namespace scone
{
	class Polynomial : public Function
	{
	public:
		Polynomial( size_t degree );
		virtual ~Polynomial();

		virtual Real GetValue( Real x ) override;
		virtual void SetCoefficient( size_t idx, Real value );
		virtual size_t GetCoefficientCount();

	protected:
		std::vector< Real > m_Coeffs;
	};
}