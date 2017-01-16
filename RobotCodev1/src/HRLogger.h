/*
 * HRLogger.h
 *
 *  Created on: Jan 8, 2017
 *      Author: 987
 */

#ifndef SRC_HRLOGGER_H_
#define SRC_HRLOGGER_H_

class HRLogger {
public:
	static void Init(bool fms);
	static void Log(const char * format,...);
};

#endif /* SRC_HRLOGGER_H_ */
