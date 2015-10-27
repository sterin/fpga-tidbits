#include <iostream>
using namespace std;

#include "TestSum.hpp"
#include "platform.h"

bool Run_TestSum(WrapperRegDriver * platform) {
	TestSum t(platform);

	cout << "Signature: " << hex << t.get_signature() << dec << endl;
	unsigned int ub = 0;
	cout << "Enter upper bound of sum: " << endl;
	cin >> ub;

	unsigned int * hostBuf = new unsigned int[ub];
	unsigned int bufsize = ub * sizeof(unsigned int);
	unsigned int golden = (ub*(ub+1))/2;

	for(unsigned int i = 0; i < ub; i++) { hostBuf[i] = i+1; }

	void * accelBuf = platform->allocAccelBuffer(bufsize);
	platform->copyBufferHostToAccel(hostBuf, accelBuf, bufsize);

	t.set_baseAddr((AccelReg) accelBuf);
	t.set_byteCount(bufsize);

	t.set_start(1);

	while(t.get_finished() != 1);

	platform->deallocAccelBuffer(accelBuf);
	delete [] hostBuf;

	AccelReg res = t.get_sum();
	cout << "Result = " << res << " expected " << golden << endl;
	t.set_start(0);
	return res == golden;
}

int main()
{
	WrapperRegDriver * platform = initPlatform();

  // TODO could autodetect correct test based on signature
  // for now, must be manually uncommented
	// Run_TestSum(platform);

	deinitPlatform(platform);

	return 0;
}
