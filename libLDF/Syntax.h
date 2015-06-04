#pragma once

#include <string>

namespace libLDF
{
	class CSyntaxCheck
	{
	public:
		CSyntaxCheck();
		~CSyntaxCheck();

		static void check(std::string line);
	};

	void check_syntax_lineEnd();
}