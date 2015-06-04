
#include <Windows.h>
#include <string>

using namespace std;

typedef HRESULT(*fnRegisterCom) ();
typedef HRESULT(*fnUnregisterCom) ();

int main(int argc, char** argv)
{
	if (argc == 2)
	{
		HMODULE h = LoadLibraryA(".\\TVOFilter.ax");
		if (h == NULL)
			return -1;
		HRESULT ret = S_OK;

		std::string arg = std::string(argv[1]);

		if (arg == std::string("register")) {

			fnRegisterCom fn = (fnRegisterCom)GetProcAddress(h, "DllRegisterServer");
			if (fn != NULL)
			{
				ret =fn();
				if (FAILED(ret))
					return -1;
			}
			else
				return -1;
		}
		else if (arg == std::string("unregister")) {

			fnUnregisterCom fn2 = (fnUnregisterCom)GetProcAddress(h, "DllUnregisterServer");
			if (fn2 != NULL) {
				ret =fn2();
				if (FAILED(ret))
					return -1;
			}
		}
		else
			return -1;

		return 0;
	}

	return -1;
}