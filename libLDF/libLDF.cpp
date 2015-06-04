
#include "libLDF.h"
#include "DashboardLayout.h"


using namespace libLDF;

extern "C" LIBLDF_API IDashboardLayout* libLDF::init_instance()
{
	try {
		IDashboardLayout* p = (IDashboardLayout*)new CDashboardLayout();
		return p;
	}
	catch (...)
	{
		throw;
		return nullptr;
	}
}


extern "C" LIBLDF_API void libLDF::release_instance(IDashboardLayout* inst)
{
	if (inst != nullptr)
		delete inst;
}