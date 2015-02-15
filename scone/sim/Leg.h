#pragma once

#include "Link.h"
#include "Types.h"
#include "../core/Vec3.h"

namespace scone
{
	namespace sim
	{
		class SCONE_SIM_API Leg
		{
		public:
			Leg( const Link& upper, const Link& foot, size_t index, Side side = NoSide, size_t rank = 0 );
			virtual ~Leg();

			const Link& GetUpperLink() const { return m_Upper; }
			const Link& GetFootLink() const { return m_Foot; }
			Side GetSide() const { return m_Side; }
			size_t GetIndex() const { return m_Index; }
			size_t GetRank() const { return m_Rank; }

			virtual Vec3 GetContactForce() const;

		private:
			size_t m_Index;
			Side m_Side;
			size_t m_Rank;
			const Link& m_Foot;
			const Link& m_Upper;
		};
	}
}