#ifndef _TEST_LOG_
#define _TEST_LOG_

//#include "C:\Users\Anrokyr\Desktop\CSChat\include\CSLog.h"
#include "..\include\CSLog.h"
#include <iostream>

namespace TEST_LOG {

	void test0() {

		CSLog log("log.txt");
		log.clearfile();

		log.SystemLog(tier0, __FILE__, __LINE__, "test0");
		log.SystemLog(tier1, __FILE__, __LINE__, "test0");
		log.SystemLog(tier2, __FILE__, __LINE__, "test0");
		log.SystemLog(tier3, __FILE__, __LINE__, "test0");

	}

	void test1() {

		CSLog log("log.txt");
	
		log.SystemLog(tier0, __FILE__, __LINE__, "test1");
		log.SystemLog(tier1, __FILE__, __LINE__, "test1");
		log.SystemLog(tier2, __FILE__, __LINE__, "test1");
		log.SystemLog(tier3, __FILE__, __LINE__, "test1");

	}

	void test2() {

		CSLog log("log.txt");

		log.UserLog(tier0, __FILE__, __LINE__, "test2");
		log.UserLog(tier1, __FILE__, __LINE__, "test2");
		log.UserLog(tier2, __FILE__, __LINE__, "test2");
		log.UserLog(tier3, __FILE__, __LINE__, "test2");

	}

	void test3() {

		CSLog log("log.txt");
	
		log.SystemLog(tier0, __FILE__, __LINE__, "test3");
		log.UserLog(tier0, __FILE__, __LINE__, "test3");
		log.SystemLog(tier1, __FILE__, __LINE__, "test3");
		log.UserLog(tier1, __FILE__, __LINE__, "test3");
		log.SystemLog(tier2, __FILE__, __LINE__, "test3");
		log.UserLog(tier2, __FILE__, __LINE__, "test3");
		log.SystemLog(tier3, __FILE__, __LINE__, "test3");
		log.UserLog(tier3, __FILE__, __LINE__, "test3");

	}


};

#endif //_TEST_LOG_