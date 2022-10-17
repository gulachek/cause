#include "gulachek/error.hpp"

namespace gulachek
{
	void error::output(std::ostream &os) const
	{
		if (is_eof())
		{
			os << "eof";
			return;
		}

		os << ss_.str();
		if (!causes_.empty())
		{
			os << " : ";
			os << causes_[0];
		}
	}

	std::ostream& operator << (std::ostream &os, const error &c)
	{
		c.output(os);
		return os;
	}
}
