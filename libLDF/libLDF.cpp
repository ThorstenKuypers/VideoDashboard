
#include "libLDF.h"
#include "Dashboard.h"

using namespace libLDF;

extern "C" LIBLDF_API IDashboardLayout* get_LDF()
{
	//try {
	IDashboardLayout* p = (IDashboardLayout*)new CDashboard();
	return p;
	//}
	//catch (...)
	//{
	//	throw;
	//	return nullptr;
	//}
}


extern "C" LIBLDF_API void release_LDF(void* inst)
{
	if (inst != nullptr)
		delete inst;
}