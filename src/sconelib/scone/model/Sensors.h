/*
** Sensors.h
**
** Copyright (C) 2013-2019 Thomas Geijtenbeek and contributors. All rights reserved.
**
** This file is part of SCONE. For more information, see http://scone.software.
*/

#pragma once

#include "Sensor.h"
#include "scone/core/types.h"
#include "scone/core/Vec3.h"
#include "Muscle.h"
#include "Dof.h"
#include "Leg.h"

#if defined(_MSC_VER)
#	pragma warning( push )
#	pragma warning( disable: 4275 )
#endif

namespace scone
{
	// Base class for muscle sensors
	struct SCONE_API MuscleSensor : public Sensor
	{
		MuscleSensor( Muscle& m ) : muscle_( m ) {}
		Muscle& muscle_;
	};

	// Sensor for normalized muscle force
	struct SCONE_API MuscleForceSensor : public MuscleSensor
	{
		MuscleForceSensor( Muscle& m ) : MuscleSensor( m ) {}
		virtual String GetName() const override { return muscle_.GetName() + ".F"; }
		virtual Real GetValue() const override { return muscle_.GetNormalizedForce(); }
	};

	// Sensor for normalized muscle length
	struct SCONE_API MuscleLengthSensor : public MuscleSensor
	{
		MuscleLengthSensor( Muscle& m ) : MuscleSensor( m ) {}
		virtual String GetName() const override { return muscle_.GetName() + ".L"; }
		virtual Real GetValue() const override { return muscle_.GetNormalizedFiberLength(); }
	};

	// Sensor for normalized muscle lengthening speed
	struct SCONE_API MuscleVelocitySensor : public MuscleSensor
	{
		MuscleVelocitySensor( Muscle& m ) : MuscleSensor( m ) {}
		virtual String GetName() const override { return muscle_.GetName() + ".V"; }
		virtual Real GetValue() const override { return muscle_.GetNormalizedFiberVelocity(); }
	};

	// Sensor that simulates Ia muscle spindle (based on [Prochazka 1999], p.135)
	struct SCONE_API MuscleSpindleSensor : public MuscleSensor
	{
		MuscleSpindleSensor( Muscle& m ) : MuscleSensor( m ) {}
		virtual String GetName() const override { return muscle_.GetName() + ".S"; }
		virtual Real GetValue() const override { return muscle_.GetNormalizedSpindleRate(); }
	};

	struct SCONE_API MuscleExcitationSensor : public MuscleSensor
	{
		MuscleExcitationSensor( Muscle& m ) : MuscleSensor( m ) {}
		virtual String GetName() const override { return muscle_.GetName() + ".excitation"; }
		virtual Real GetValue() const override { return muscle_.GetExcitation(); }
	};

	struct SCONE_API MuscleActivationSensor : public MuscleSensor
	{
		MuscleActivationSensor( Muscle& m ) : MuscleSensor( m ) {}
		virtual String GetName() const override { return muscle_.GetName() + ".A"; }
		virtual Real GetValue() const override { return muscle_.GetActivation(); }
	};

	// Base struct for dof sensors
	struct SCONE_API DofSensor : public Sensor
	{
		DofSensor( Dof& dof, Dof* root_dof ) : dof_( dof ), root_dof_( root_dof ) {}
		Dof& dof_;
		Dof* root_dof_;
	};

	struct SCONE_API DofPositionSensor : public DofSensor
	{
		DofPositionSensor( Dof& dof, Dof* root_dof = nullptr ) : DofSensor( dof, root_dof ) {}
		virtual String GetName() const override { return dof_.GetName() + ".DP"; }
		virtual Real GetValue() const override;
	};

	struct SCONE_API DofVelocitySensor : public DofSensor
	{
		DofVelocitySensor( Dof& dof, Dof* root_dof = nullptr ) : DofSensor( dof, root_dof ) {}

		virtual String GetName() const override { return dof_.GetName() + ".DV"; }
		virtual Real GetValue() const override;
	};

	struct SCONE_API DofPosVelSensor : public DofSensor
	{
		DofPosVelSensor( Dof& dof, double kv, Dof* root_dof = nullptr ) : DofSensor( dof, root_dof ), kv_( kv ) {}
		virtual String GetName() const override { return dof_.GetName() + ".DPV"; }
		virtual Real GetValue() const override;
		double kv_;
	};

	// Sensor for normalized leg load, based on target_area
	struct SCONE_API LegLoadSensor : public Sensor
	{
		LegLoadSensor( const Leg& leg ) : leg_( leg ) {}

		virtual String GetName() const override { return leg_.GetName() + ".LD"; }
		virtual Real GetValue() const override { return leg_.GetLoad(); }
		const Leg& leg_;
	};

	// Base struct for body sensors
	struct SCONE_API BodyPointSensor : public Sensor
	{
		BodyPointSensor( Body& body, Vec3 ofs, Vec3 dir ) : body_( body ), offset_( ofs ), direction_( dir ) {}
		Body& body_;
		Vec3 offset_;
		Vec3 direction_;
	};

	struct SCONE_API BodyPointPositionSensor : public BodyPointSensor
	{
		BodyPointPositionSensor( Body& body, Vec3 ofs, Vec3 dir ) : BodyPointSensor( body, ofs, dir ) {}
		virtual String GetName() const override;
		virtual Real GetValue() const override;
	};

	struct SCONE_API BodyPointVelocitySensor : public BodyPointSensor
	{
		BodyPointVelocitySensor( Body& body, Vec3 ofs, Vec3 dir ) : BodyPointSensor( body, ofs, dir ) {}
		virtual String GetName() const override;
		virtual Real GetValue() const override;
	};

	struct SCONE_API BodyPointAccelerationSensor : public BodyPointSensor
	{
		BodyPointAccelerationSensor( Body& body, Vec3 ofs, Vec3 dir) : BodyPointSensor( body, ofs, dir ) {}
		virtual String GetName() const override;
		virtual Real GetValue() const override;
	};
}

#if defined(_MSC_VER)
#	pragma warning( pop )
#endif
