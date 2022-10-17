#ifndef GULACHEK_ERROR_HPP
#define GULACHEK_ERROR_HPP

#include <cstddef>
#include <vector>
#include <sstream>
#include <type_traits>

namespace gulachek
{
	template <typename T>
	concept user_defined_codeable = requires (T val)
	{
		{static_cast<std::size_t>(val)};
	};

	class GULACHEK_ERROR_API error
	{
		enum class standard_code
		{
			generic,
			eof,
			stub // (not implemented)
		};

		public:
			error(const char* desc = "") :
				code_{standard_code::generic},
				ucode_{},
				ss_{desc}
			{}

			template <user_defined_codeable T>
			error(T ucode, const char* desc = "") :
				code_{standard_code::generic},
				ucode_{static_cast<std::size_t>(ucode)},
				ss_{desc}
			{}

			error(const error &other) :
				code_{other.code_},
				ucode_{other.ucode_},
				ss_{other.ss_.str()},
				causes_{other.causes_}
			{}

			error(error &&other) = default;
			error& operator =(error &&other) = default;

			error& operator = (const error &other)
			{
				error temp{other};
				std::swap(*this, temp);
				return *this;
			}

			operator bool () const
			{
				return (code_ != standard_code::generic) ||
					ucode_ ||
					ss_.str().size();
			}

			static error eof()
			{
				error out;
				out.code_ = standard_code::eof;
				return out;
			}

			template <user_defined_codeable T>
			void ucode(T c)
			{ ucode_ = static_cast<std::size_t>(c); }

			template <user_defined_codeable T>
			bool has_ucode(T c) const
			{ return ucode_ == static_cast<std::size_t>(c); }

			template <typename T>
			error& format(const T &rhs)
			{
				ss_ << rhs;
				return *this;
			}

			bool is_eof() const
			{ return code_ == standard_code::eof; }

			template <typename T>
			error& operator << (const T &rhs)
			{ return format<T>(rhs); }

			template <typename Cause>
			void add_cause(Cause &&c)
			{ causes_.emplace_back(std::forward<Cause>(c)); }

			void output(std::ostream &os) const;

		private:
			standard_code code_;
			std::size_t ucode_;
			std::stringstream ss_;
			std::vector<error> causes_;
	};

	template <typename T>
	concept __is_error_ref = std::is_same_v<T, error&>;

	template <typename T>
	concept error_writable = requires (T val, error c)
	{
		{c << val} -> __is_error_ref;
	};

	GULACHEK_ERROR_API std::ostream& operator << (std::ostream &os, const error &c);
}

#endif
