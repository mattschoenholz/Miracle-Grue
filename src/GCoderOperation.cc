/**
   MiracleGrue - Model Generator for toolpathing. <http://www.grue.makerbot.com>
   Copyright (C) 2011 Far McKon <Far@makerbot.com>, Hugo Boyer (hugo@makerbot.com)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

*/
#include <assert.h>
#include <sstream>

#include "GCoderOperation.h"

#include "json-cpp/include/json/value.h"

using namespace std;
using namespace Json;

/************** Start of Functions custom to this <NAME_OF>Operation ***********************/

// local function that adds an s to a noun if count is more than 1
std::string plural(const char*noun, int count, const char* ending = "s")
{
	string s(noun);
	if (count>1)
	{
		return s+ending;
	}
	return s;
}

// writes a linear gcode movement
void moveTo(std::ostream &ss,  double x, double y, double z, double feed, const char*comment = NULL)
{
	std::string msg;
	if(comment)
	{
		msg = " (";
		msg += comment;
		msg += ")";
	}
	ss << "G1 X" << x << " Y" << y << " Z" << z << " F" << feed << msg  << endl;
}

// writes an extruder reversal gcode snippet
void reverseExtrude(std::ostream &ss, double feedrate)
{
	ss << "M108 R" << feedrate << endl;
	ss << "M102 (reverse)" << endl;
}



/************** End of Functions custom to this <NAME_OF>Operation ***********************/


/// This staic global pointer points to a unique instance of a
/// Value object, which contains the minimum configuration values
/// required to build a working operation of this type.
/// Must be named <OperationName>ConfigRequirements so it does not collide with
/// other global static values.
static Value* GCoderOperationConfigRequirements;


/**
 * This is an accessor function to retreve a singleton of <OperationName>ConfigRequirements
 * if that dictionary does not exit, this function will generate it.
 * The returned dictionary specifies the minimum configuration settings needed to make a valid
 * initaliation of an Operation of this type
 *
 * @return global static Value pointer to configuration dictionary.
 */
Value* GCoderOperation::getStaticConfigRequirements()
{
	// if we don't have one of these global static's, we have never initalized,
	// so initalize now.
	if (GCoderOperationConfigRequirements == 0x00)
	{
		// - Start custom to GCoderOperation code
		// for this Example operation, we need a prefix and a language specified
		// to initalize
		Value* cfg = new Value;
//		( *cfg )["prefix"]= "asString";
//		( *cfg )["lang"] = "asString";
		// - End custom to GCoderOperation code
		GCoderOperationConfigRequirements = cfg;

		std::cout << __FUNCTION__ << std::endl;
		std::cout << "TODO: Create config Requirements" << std::endl;

		// This object is expected to live until the program dies. No deconstruction !
	}
	return GCoderOperationConfigRequirements;
}


/**
 * Standard Constructor.  Note that an object can be built and exist, but
 * not yet be configured or initalized. See details in implementation.
 * Anything things that do not need configuration
 * SHOULD be initalized in the constructor.
 */

GCoderOperation::GCoderOperation()
{
	// - Start custom to GCoderOperation code
	// Because this logging stream is always this file (and is not configuration dependant,
	//we build it in the constructor, and destroy it  in the destructor.

	this->acceptTypes.push_back(/*AtomType*/TYPE_BGL_MESH);
	//this->emitTypes.push_back(); Left in as a note, but this example emits no data type

	// - End custom to GCoderOperation code

}


/**
 * Standard Destructor.  This should close streams (if any are open) and
 * deinitalize the Operation (if it is still initalized). See details in implementation.
 */
GCoderOperation::~GCoderOperation()
{
	// IFF we are currently initalized, we need to check for an open stream, as well as deinit
	if(initalized) {

		// - Following line custom to GCoderOperation code
		cout << " Operation initailzed at destruction time. Automatically running deinit" <<endl;

//		//NOTE:  deinit will check that the data stream is closed, and if needed it will
//		// gaurentee a last DataEnvelope is sent (so we can deinitalize with confidence)
//		this->deinit();
	}

	// - Start custom to GCoderOperation code

	// - End custom to GCoderOperation code


}



/**
 * This function takes a configuration object, and verifies that it can be used to
 * configure the object.
 * @param config
 * @return
 */
bool GCoderOperation::isValidConfig(Configuration& config) const
{

	cout << __FUNCTION__ << endl;

	if(config["GCoderOperation"].type() !=  /*ValueType.*/objectValue)
	{
		// - Start custom to GCoderOperation code
		/// TODO:
		/// req = getStaticConfigRequirements():
		/// if (req >= config)
			///return true
		// - End custom to GCoderOperation code
		return true;
	}
	cout << "ERROR: configuration is not valid, In BETA accepting config anyway" << endl;
	return true;
}


/**
 * This function initalizes and configures this Operation to take data. Once this returns,
 * the operation may be sent data envelopes (via 'accept') at any time.  All streams, memory structures,
 * and related infrastructure for a specific processing setup must be created here.
 * @param config a configuration for setting up this operation
 * @param outputs a list of other Operations to send out outgoing packets to
 */
void GCoderOperation::init(Configuration& config,const std::vector<Operation*> &outputs)
{
	bool baseSuccess = Operation::initCommon(config, outputs);
	if(baseSuccess){
	// - Start custom to GCoderOperation code

	// - End custom to GCoderOperation code
	}

}


/**
 * This function tears down everything setup in initalization. It will also check to see
 * if a stream is running, and if it is, it forces a final data envelope to be queued before
 * continuing with deiitalization. This will force a data flush in edge or fail cases.
 */
void GCoderOperation::deinit()
{
	assert(this->initalized == true);
	assert(this->pConfig != NULL);

	// IFF we have an ongoing stream, as we are being deinitalized, something went wrong.
	// force a final DataEnvelpe to our operation to flush the end of the stream.
	if(streamRunning)
	{
		// - Following line custom to GCoderOperation code
		cout << "Stream Running at deinit time. Automatically sending final envelope " <<endl;

/*
		DataEnvelope d;
		d.setFinal();
		this->accept(d);
		this->streamRunning = false;
		d.release(); //release the constuctor default ref count of 1
*/

	}

	// - Start custom to GCoderOperation code

	// - End custom to GCoderOperation code

	this->initalized = false;
	pConfig = NULL;
}

/**
 * This is the heart of envelope processing.
 * @param envelope
 */
void GCoderOperation::processEnvelope(const DataEnvelope& envelope)
{
	/// we should be configured before ever doing this
	assert(this->initalized == true);

	/// If this stream is not running, our first packet starts it running
	if( this->streamRunning == false) {
		this->streamRunning = true;
	}

	// - Start custom to GCoderOperation code

	cout << "TODO: test cast and/or flag type in GCoderOperation::processEnvelope" << endl;

	stringstream ss;
	const PathData &pathData = *(dynamic_cast<const PathData* > (&envelope) );
	writePaths(ss, pathData);
	wrapAndEmit(ss.str().c_str());

	cout << "TODO: test cast and/or flag type in GCoderOperation::processEnvelope" << endl;

	// - End custom to GCoderOperation code

	return;
}

void GCoderOperation::start(){

	// the first thing that needs to happen in start is the propigation of the
	// start to all other modueles.
	Operation::startCommon();


	// - Start custom to GCoderOperation code
	cout << "GCoderOperation::start() !!" << endl;
	stringstream ss;

	writeGCodeConfig(ss);
	writeMachineInitialization(ss);
	writeExtrudersInitialization(ss);
	writePlatformInitialization(ss);

	writeHomingSequence(ss);
	writeWarmupSequence(ss);
	writeAnchor(ss);

	const char *msg = ss.str().c_str();
	wrapAndEmit(msg);
	// - End custom to GCoderOperation code

}

void GCoderOperation::finish(){

	// - Start custom to GCoderOperation code
	cout << "GCoderOperation::finish()"<< endl;
	stringstream ss;
	writeGcodeEndOfFile(ss);
	const char *msg = ss.str().c_str();
	wrapAndEmit(msg);

	// - End custom to GCoderOperation code


	// the last things to do in finish is propigate the finish
	// to all other modules.
	Operation::finishCommon();

}


/************** Start of Functions custom to GCoderOperation ***********************/
void GCoderOperation::writePaths(ostream& ss, const PathData& pathData) const
{
	const Configuration &config =  configuration();
	 // distance above mid layer position of extrusion

	cout << endl << "GCoderOperation::writePaths()" << endl;
	int extruderCount = pathData.paths.size();
	ss << "(PATHS for: " << extruderCount << plural("Extruder", extruderCount) << ")"<< endl;

	int extruderId = 0;
//	for(std::vector<Paths>::const_iterator extruderIt = pathData.paths.begin(); extruderIt != pathData.paths.end(); extruderIt++)
//	{
//		if (pathData.paths.size() > 0)
//		{
//			writeSwitchExtruder(ss, extruderId);
//		}
//		// to each extruder its speed
//		double z = pathData.positionZ + config.extruders[extruderId].nozzleZ();
//		double reversal = config.extruders[extruderId].reversalExtrusionSpeed();
//		double pathFeedrate = config.scalingFactor * config.extruders[extruderId].fastFeedRate();
//		double extrusionSpeed = config.scalingFactor * config.extruders[extruderId].fastExtrusionSpeed();
//
//		const Paths &paths = *extruderIt;
//		for (Paths::const_iterator pathIt = paths.begin() ; pathIt != paths.end();  pathIt ++)
//		{
//			const Polygon &polygon = *pathIt;
//			ss << "(  POLYGON " << polygon.size() << " Points)" << endl;
//			for(Polygon::const_iterator i= polygon.begin(); i!= polygon.end(); i++ )
//			{
//				const Point2D &p = *i;
//				ss << "(      POINT [" << p.x << ", " << p.y << "] )" << endl;
//				moveTo(ss, p.x, p.y, z, pathFeedrate);
//			}
//		}
//		reverseExtrude(ss, reversal);
//		ss << endl;
//		if (pathData.paths.size() > 0)
//		{
//			writeWipeExtruder(ss, extruderId);
//		}
//		extruderId ++;
//	}
}

void GCoderOperation::writeSwitchExtruder(ostream& ss, int extruderId) const
{

	ss << "( extruder " << extruderId << " )" << endl;
	ss << "( GSWITCH T" << extruderId << " )" << endl;
	ss << endl;
}

void GCoderOperation::writeWipeExtruder(ostream& ss, int extruderId) const
{
	ss << "( GWIPE my extruder #" << extruderId << " )"<< endl;
	ss << endl;
}

void GCoderOperation::writeGCodeConfig(std::ostream &ss) const
{
	const Configuration &config = configuration();

	ss << endl;
	ss << "(Makerbot Industries 2011)" << endl;
	ss << "(This file contains digital fabrication directives in gcode format)"<< endl;
	ss << "(What's gcode? http://wiki.makerbot.com/gcode)" <<  endl;
	ss << "(For your 3D printer)" << endl;
	config.writeGcodeConfig(ss, "* ");
	ss << endl;
}

void GCoderOperation::writeMachineInitialization(std::ostream &ss) const
{
	const Configuration &config = configuration();

	ss <<  "G21 (set units to mm)" << endl;
	ss <<  "G90 (absolute positioning mode)" << endl;

	int toolHeadId = 0;

//	if (config.extruders.size() > 1)
//	{
//		for (std::vector<Extruder>::const_iterator i= config.extruders.begin(); i!=config.extruders.end(); i++)
//		{
//			Extruder e = *i;
//			int coordinateSystemNb = toolHeadId +1;
//			ss << "G10 P" << coordinateSystemNb << " X" <<  e.coordinateSystemOffsetX() << " Y0 Z-0.3" << endl;
//			toolHeadId ++;
//		}
//	}
	ss << endl;
}


void GCoderOperation::writeExtrudersInitialization(std::ostream &ss) const
{
	const Configuration &config = configuration();
	string plural = "";
//	if(config.extruders.size()>1) plural = "s";
//	ss << "(setup extruder" << plural <<")" <<endl;
	int toolHeadId = 0;

//	for (std::vector<Extruder>::const_iterator i= config.extruders.begin(); i!=config.extruders.end(); i++)
//	{
//		double t = 999;
//		Extruder e = *i;
//		ss << "M103 T" << toolHeadId << " (Make sure motor for extruder " << toolHeadId << " is stopped)" << endl;
//		ss << "M108 R" << e.defaultExtrusionSpeed() << " T" << toolHeadId << " (set extruder " <<  toolHeadId << " speed to the default " << e.defaultExtrusionSpeed() << " RPM)" << endl;
//		ss << "M104 S" << e.defaultExtrusionSpeed()  << " T" << toolHeadId << " (set temperature of extruder " << toolHeadId <<  " to "  << e.extrusionTemperature() << " degrees Celsius)" << endl;
//		ss << endl;
//		toolHeadId ++;
//	}
	ss << endl;
}


void GCoderOperation::writePlatformInitialization(std::ostream &ss) const
{
	Configuration config = configuration();

	const double t = config["platform"]["temperature"].asDouble();
	ss << "M109 S" << t << " T0 (heat the build-platform to "  << t << " Celsius)" << endl;
	ss << endl;

}

void GCoderOperation::writeHomingSequence(std::ostream &ss) const
{
	const Configuration &config = configuration();

	ss << endl;
	ss << "(go to home position)" << endl;
	ss << "G162 Z F800 (home Z axis maximum)" << endl;
	ss << "G92 Z5 (set Z to 5)" << endl;
	ss << "G1 Z0.0 (move Z down 0)" << endl;
	ss << "G162 Z F100 (home Z axis maximum)" << endl;
	ss << "G161 X Y F2500 (home XY axes minimum)" << endl;
	ss << "M132 X Y Z A B (Recall stored home offsets for XYZAB axis)" << endl;
//	if (config.extruders.size() > 1)
//		ss << "G54 (first work coordinate system)" << endl;
	ss << endl;
}

void GCoderOperation::writeWarmupSequence(std::ostream &ss) const
{
	const Configuration &config = configuration();

	ss << endl;

/*	for (int i=0; i< config.extruders.size(); i++)
	{
		moveTo(ss, 	config.platform.waitingPositionX(),
					config.platform.waitingPositionY(),
					config.platform.waitingPositionZ(),
					config.extruders[i].fastFeedRate(),
					"go to waiting position" );
	}*/
//
//	for (int i=0; i< config.extruders.size(); i++)
//	{
//		ss << "M6 T" << i << " (wait for tool " << i<<" to reach temperature)" << endl;
//	}
//	ss << "(heated build platform temperature is tied to tool 0 for now)" << endl;
//	ss << endl;
//	ss << endl;ls

}

void GCoderOperation::writeGcodeEndOfFile(std::ostream &ss) const
{
	const Configuration &config = configuration();

//	for (int i=0; i< config.extruders.size(); i++)
//	{
//		ss << "M104 S0 T" << i << " (set extruder temperature to 0)" << endl;
//		ss << "M109 S0 T" << i << " (set heated-build-platform temperature to 0)" << endl;
//	}

	ss << "G162 Z F500 (home Z axis maximum)" << endl;
	ss << "(That's all folks!)" << endl;
}


void GCoderOperation::writeAnchor(std::ostream &ss) const
{
	const Configuration &config = configuration();
	ss << "(Create Anchor)" << endl;
	// moveTo(ss, config.platform.waitingPositionX, config.platform.waitingPositionY, config.platform.waitingPositionZ, config.fastFeed, "nozzle down" );
	ss << "G1 Z0.6 F300    (Position Height)" << endl;
	ss << "M108 R4.0   (Set Extruder Speed)" << endl;
	ss << "M101        (Start Extruder)" << endl;
	ss << "G4 P1500" << endl;
	ss << endl;
}


/// Wraps a string into a msg file and automatically emits it.
void GCoderOperation::wrapAndEmit(const char* msg)
{
	cout << endl;
	cout << "********************************** GCODE DATA ***************************" << endl;
	cout << msg;
	cout << "*************************************** END *****************************" << endl;
	cout << endl;

	Operation::emit(new GCodeEnvelope(msg));
}




/************** End of Functions custom to GCoderOperation ***********************/


