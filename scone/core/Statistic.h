#pragma once

#include "core.h"

namespace scone
{
	template< typename T = Real >
	class Statistic
	{
	public:
		enum InterpolationMode { NoInterpolation, LinearInterpolation };

		Statistic( InterpolationMode im = LinearInterpolation ) : m_InterpolationMode( im ) { Reset(); }

		void AddSample( const T& value, const double& timestamp )
		{
			if ( m_nSamples == 0 )
			{
				// first sample is special case when interpolating
				SCONE_CONDITIONAL_THROW( m_InterpolationMode == LinearInterpolation && timestamp != 0.0, "First timestamp needs to be 0 when using interpolation" );
				m_PrevValue = m_Initial = m_Highest = m_Lowest = value;
			}

			// update min / max
			m_Highest = std::max( m_Highest, value );
			m_Lowest = std::min( m_Lowest, value );

			// update average
			double dt = timestamp - m_PrevTime;
			switch( m_InterpolationMode )
			{
			case NoInterpolation: m_Total += dt * value; break;
			case LinearInterpolation: m_Total += dt * ( m_PrevValue + value ) / 2.0; break;
			}

			// update previous values for next call
			m_PrevTime = timestamp;
			m_PrevValue = value;
			m_nSamples++;
		}

		void Reset()
		{
			m_PrevValue = m_Total = m_Highest = m_Lowest = 0.0;
			m_PrevTime = 0.0;
			m_nSamples = 0;
		}

		double GetPrevTime() { return m_PrevTime; }

		T GetAverage() const
		{
			SCONE_CONDITIONAL_THROW( m_nSamples == 0, "Cannot perform operation, no samples have been added" );

			if ( m_PrevTime == 0.0 )
				return m_Initial; // only one sample was added
			else return m_Total / m_PrevTime;
		}

		T GetHighest() const { return m_Highest; }
		T GetLowest() const { return m_Lowest; }
		T GetInitial() const { return m_Initial; }
		T GetLatest() const { return m_PrevValue; }
		T GetTotal() const { return m_Total; }

	private:
		T m_Total, m_Initial, m_Highest, m_Lowest, m_PrevTime, m_PrevValue;
		InterpolationMode m_InterpolationMode;
		size_t m_nSamples;
	};

	typedef Statistic< Real > MeasuredReal;
}