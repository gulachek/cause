#define BOOST_TEST_MODULE ErrorTest
#include <boost/test/unit_test.hpp>

#include <gulachek/error.hpp>

using gulachek::error;

enum class ucode
{
	success,
	first,
	second
};

BOOST_AUTO_TEST_CASE(DefaultErrorIsFalsy)
{
	error err;
	if (err)
	{
		BOOST_FAIL("Default error should be falsy");
	}
}

BOOST_AUTO_TEST_CASE(ErrorWithFormatIsTruthy)
{
	error err;
	err << "a message";

	if (!err)
	{
		BOOST_FAIL("Formatted errors should be truthy");
	}
}

BOOST_AUTO_TEST_CASE(ErrorConstructedWithMessageIsTruthy)
{
	error err{"a message"};

	if (!err)
	{
		BOOST_FAIL("Formatted errors should be truthy");
	}
}

BOOST_AUTO_TEST_CASE(ZeroUcodeIsNotError)
{
	error err{ucode::success};
	if (err)
	{
		BOOST_FAIL("Zero ucode should be falsy");
	}
}

BOOST_AUTO_TEST_CASE(ErrorMessageHasFmt)
{
	error err;
	err << "Message " << 1;
	std::string msg{err.what()};
	BOOST_TEST(msg == "Message 1");
}

BOOST_AUTO_TEST_CASE(CanThrowError)
{
	try
	{
		error err{"hello world"};
		throw err;
	}
	catch (const std::exception &ex)
	{
		std::string msg{ex.what()};
		BOOST_TEST(msg == "hello world");
	}
}

BOOST_AUTO_TEST_CASE(CauseIncludesErrorMessage)
{
	error err{"hello world"};
	error cause{"the cause"};
	err.add_cause(cause);
	std::string msg{err.what()};

	BOOST_TEST(msg.find("hello world") != std::string::npos);
	BOOST_TEST(msg.find("the cause") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(WrappingACauseHasBothMessages)
{
	error cause{"the cause"};
	auto err = cause.wrap() << "hello world";

	std::string msg{err.what()};

	BOOST_TEST(msg.find("hello world") != std::string::npos);
	BOOST_TEST(msg.find("the cause") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(EofIsEof)
{
	auto eof = error::eof();
	BOOST_TEST(eof.is_eof());
}

BOOST_AUTO_TEST_CASE(CopyErrorIsStillEof)
{
	auto eof = error::eof();
	error copy = eof;
	BOOST_TEST(copy.is_eof());
}

BOOST_AUTO_TEST_CASE(CopyAssignmentErrorIsStillEof)
{
	auto eof = error::eof();
	error copy;
	copy = eof;
	BOOST_TEST(copy.is_eof());
}
