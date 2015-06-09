
#include "libLDF.h"
#include "DashboardLayout.h"


using namespace libLDF;

extern "C" LIBLDF_API IDashboardLayout* get_LDF()
{
	//try {
		IDashboardLayout* p = (IDashboardLayout*)new CDashboardLayout();
		return p;
	//}
	//catch (...)
	//{
	//	throw;
	//	return nullptr;
	//}
}


extern "C" LIBLDF_API void release_LDF(IDashboardLayout* inst)
{
	if (inst != nullptr)
		delete inst;
}