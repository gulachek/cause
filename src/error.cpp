#include "gulachek/error.hpp"

namespace gulachek
{
	/*
	error& error::operator = (const error &other)
	{
		error temp{other};
		std::swap(*this, temp);
		return *this;
	}
	*/

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

	const char *error::what() const noexcept
	{
		std::ostringstream ss;
		ss << *this;
		freeze_msg_ = ss.str();
		return freeze_msg_.c_str();
	}

	std::ostream& operator << (std::ostream &os, const error &c)
	{
		c.output(os);
		return os;
	}

	error error::eof()
	{
		error out;
		out.code_ = standard_code::eof;
		return out;
	}

	error::operator bool () const
	{
		return (code_ != standard_code::generic) ||
			ucode_ ||
			ss_.str().size();
	}

	bool error::is_eof() const
	{ return code_ == standard_code::eof; }

	error error::wrap() const
	{
		error wrapper;
		wrapper.add_cause(*this);
		return wrapper;
	}
}
