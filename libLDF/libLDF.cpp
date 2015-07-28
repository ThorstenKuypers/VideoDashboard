
#include "libLDF.h"
#include "Dashboard.h"

using namespace libLDF;

IDashboardLayout* LayoutInstanceFactory::GetDashboardLayoutInst()
{
	IDashboardLayout* ptr = nullptr;

	try {
		ptr = static_cast<IDashboardLayout*>(new CDashboard());
	}
	catch (std::exception e)
	{
		throw e;
	}

	return ptr;
}

void LayoutInstanceFactory::ReleaseDashboardLayoutInstance(IDashboardLayout* inst)
{
	CDashboard* ptr = static_cast<CDashboard*>(inst);

	if (ptr != nullptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}