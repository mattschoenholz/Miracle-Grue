/**
   MiracleGrue - Model Generator for toolpathing. <http://www.grue.makerbot.com>
   Copyright (C) 2011 Far McKon <Far@makerbot.com>, Hugo Boyer (hugo@makerbot.com)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

*/
#ifndef GCODE_OPERATION_H_
#define GCODE_OPERATION_H_

#include <iostream>
#include <fstream>

#include <string>
#include <assert.h>

#include "Operation.h"
#include "PathData.h"

#include "GCodeEnvelope.h"

/**
 * GCoderOperation creates gcode from a stream of path envelopes.
 *
 */
class GCoderOperation : public Operation
{

/************** Start of Functions each <NAME_OF>Operation must contain***********************/
public:

	///Standard Constructor
	GCoderOperation();


	///Standard Destructor
	~GCoderOperation();


	/**
	 * This function returns a global static pointer to a list of Configuration Requirements
	 * See details in implementation
	 * @return global static Json::Value pointer
	 */
	static Json::Value* getStaticConfigRequirements();

	/**
	 * This is the heart of data processing.  This is the core of the envelope accepting system.
	 * See details in implementation
	 * @param envelope reference to a DataEnvelope, or related subclass
	 */
	void processEnvelope(const DataEnvelope& envelope);

	/**
	 * This initalizes an operation with specific settings for processing this stream.
	 * See details in implementation
	 * @param config configuration for this stream
	 * @param outputs A vector of operations that receive output envelopes of data from this object
	 */
	void init(Configuration& config,const std::vector<Operation*> &outputs);

	///This un-initalizes an operation.
	void deinit();

	///This function must be called to start the processing chain
	void start();

	///This function must be called to wrap up the processing chain
	void finish();

/************** End of Functions each <NAME_OF>Operation must contain***********************/


/************** Start of Functions custom to this <NAME_OF>Operation ***********************/
private:
	bool isValidConfig(Configuration& config) const;

	// write important config information in gcode file
    void writeGCodeConfig(std::ostream &ss) const;
	void writeMachineInitialization(std::ostream &ss) const;
    void writePlatformInitialization(std::ostream &ss) const;
    void writeExtrudersInitialization(std::ostream &ss) const;
    void writeHomingSequence(std::ostream &ss) const;
    void writeWarmupSequence(std::ostream &ss) const;
    void writeAnchor(std::ostream &ss) const;

    void writePaths(std::ostream &ss, const PathData& pathData) const;

    void writeSwitchExtruder(std::ostream& ss, int extruderId) const;
    void writeWipeExtruder(std::ostream& ss, int extruderId) const;

    void writeGcodeEndOfFile(std::ostream &ss) const;

	void wrapAndEmit(const char* msg);


/************** End of Functions custom to this <NAME_OF>Operation ***********************/

};




#endif /* GCODE_OPERATION_H_ */

