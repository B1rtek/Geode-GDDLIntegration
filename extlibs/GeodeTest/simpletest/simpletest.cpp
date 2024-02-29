#include "simpletest.h"
#include <stdio.h>
#include <string.h>
#include <cstdint>
#include <stdarg.h>
#include <time.h>
#include <format>
#include <iostream>

//---------------------------------------------------------------------------------
// statics
//---------------------------------------------------------------------------------
void DefaultPrint(char const* string) { printf("%s", string); }

TestFixture* TestFixture::ourFirstTest;
TestFixture* TestFixture::ourLastTest;
thread_local TestFixture* TestFixture::ourCurrentTest;
void (*TestFixture::Print)(char const* string) = DefaultPrint;

//---------------------------------------------------------------------------------
// Standard type printers
//---------------------------------------------------------------------------------
TempString::TempString(const TempString& other)
{
	if (other.myTextPointer == other.myTextBuffer)
	{
		strcpy(myTextBuffer, other.myTextBuffer);
		myTextPointer = myTextBuffer;
	}
	else
	{
		myTextPointer = other.myTextPointer;
	}
}
TempString TypeToString(int value)
{
	TempString tempString;
	if (TestFixture::GetCurrentTest()->GetPrintMethod() == TestFixture::PrintHexadecimal)
		snprintf(tempString.myTextBuffer, STRING_LENGTH, "0x%08X", value);
	else
		snprintf(tempString.myTextBuffer, STRING_LENGTH, "%d", value);
	return tempString;
}
TempString TypeToString(unsigned int value)
{
	TempString tempString;
	if (TestFixture::GetCurrentTest()->GetPrintMethod() == TestFixture::PrintHexadecimal)
		snprintf(tempString.myTextBuffer, STRING_LENGTH, "0x%08X", value);
	else
		snprintf(tempString.myTextBuffer, STRING_LENGTH, "%u", value);
	return tempString;
}
TempString TypeToString(long value)
{
	TempString tempString;
	if (TestFixture::GetCurrentTest()->GetPrintMethod() == TestFixture::PrintHexadecimal)
		snprintf(tempString.myTextBuffer, STRING_LENGTH, "0x%016lX", value);
	else
		snprintf(tempString.myTextBuffer, STRING_LENGTH, "%ld", value);
	return tempString;
}
TempString TypeToString(unsigned long value)
{
	TempString tempString;
	if (TestFixture::GetCurrentTest()->GetPrintMethod() == TestFixture::PrintHexadecimal)
		snprintf(tempString.myTextBuffer, STRING_LENGTH, "0x%016lX", value);
	else
		snprintf(tempString.myTextBuffer, STRING_LENGTH, "%lu", value);
	return tempString;
}
TempString TypeToString(long long value)
{
	TempString tempString;
	if (TestFixture::GetCurrentTest()->GetPrintMethod() == TestFixture::PrintHexadecimal)
		snprintf(tempString.myTextBuffer, STRING_LENGTH, "0x%016llX", value);
	else
		snprintf(tempString.myTextBuffer, STRING_LENGTH, "%lld", value);
	return tempString;
}
TempString TypeToString(unsigned long long value)
{
	TempString tempString;
	if (TestFixture::GetCurrentTest()->GetPrintMethod() == TestFixture::PrintHexadecimal)
		snprintf(tempString.myTextBuffer, STRING_LENGTH, "0x%016llX", value);
	else
		snprintf(tempString.myTextBuffer, STRING_LENGTH, "%llu", value);
	return tempString;
}
TempString TypeToString(float value)
{
	TempString tempString;
	snprintf(tempString.myTextBuffer, STRING_LENGTH, "%.16g", value);
	return tempString;
}
TempString TypeToString(double value)
{
	TempString tempString;
	snprintf(tempString.myTextBuffer, STRING_LENGTH, "%.16g", value);
	return tempString;
}
TempString TypeToString(bool value)
{
	return TempString(value ? "true" : "false");
}
TempString TypeToString(char const* value)
{
	return TempString(value ? value : "(nullptr)");
}
TempString TypeToString(void const* value)
{
	if (value == nullptr)
		return TempString("(nullptr)");
	TempString tempString;
	snprintf(tempString.myTextBuffer, STRING_LENGTH, "0x%p", value);
	return tempString;
}
TempString TypeToString(void const* value, char const* extra)
{
	if (value == nullptr)
		return TempString("(nullptr)");
	TempString tempString;
	snprintf(tempString.myTextBuffer, STRING_LENGTH, "(0x%p) %s", value, extra);
	return tempString;
}

//---------------------------------------------------------------------------------
// Fixture implementation
//---------------------------------------------------------------------------------
TestFixture::TestFixture()
	: myNextTest(nullptr)
	, myNextError(nullptr)
	, myNumTestsChecked(0)
	, myNumErrors(0)
	, myPrintMethod(PrintDefault)
{
	// global link list registration, add in order of discovery
	if (ourFirstTest == nullptr)
	{
		ourFirstTest = this;
		ourLastTest = this;
	}
	else
	{
		ourLastTest->myNextTest = this;
		ourLastTest = this;
	}
}
//---------------------------------------------------------------------------------
bool TestFixture::ExecuteTest()
{
	myNumTestsChecked = myNumErrors = 0;
	myNextError = (TestError*)myMessageSpace;

	TestFixture* lastCurrent = ourCurrentTest;
	ourCurrentTest = this;
	Setup();
	RunTest();
	TearDown();
	ourCurrentTest = lastCurrent;

	return myNumErrors == 0;
}
//---------------------------------------------------------------------------------
// Utility to print a part of a string to show where the error is and put elipse
// where the string is truncated
//---------------------------------------------------------------------------------
static void locCopyStringWithElipse(char dest[STRING_EQ_PRINT_LENGTH], char const* string, size_t offset = 0)
{
	char const* start = string + offset - STRING_EQ_PRINT_LENGTH / 2;
	if (start < string)
		start = string;

	int i = 0;
	for (; i < STRING_EQ_PRINT_LENGTH - 1 && start[i]; ++i)
	{
		if (i < 3 && start > string)
			dest[i] = '.';
		else if (start[i] == '\r' || start[i] == '\n' || start[i] == '\t')
			dest[i] = '\\'; // simply replace this with '\', we're just aiming for a general idea not an exact representation
		else
			dest[i] = start[i];
	}

	dest[i] = 0;

	if (i == STRING_EQ_PRINT_LENGTH - 1 && start[i])
	{
		dest[i - 1] = '.';
		dest[i - 2] = '.';
		dest[i - 3] = '.';
	}
}
//---------------------------------------------------------------------------------
// Instead of just check for error and printing the string, try go be smart about
// how the information is written out:
// ... quick brown fox jumps over ...
//                      ^
// ... quick brown fox jamps over ...
//---------------------------------------------------------------------------------
bool TestFixture::TestStrings(char const* left, char const* right, char const* prefix, char const* condition)
{
	AddTest();
	if (left == right)
	{
		return true;
	}

	char leftLine[STRING_EQ_PRINT_LENGTH];
	char rightLine[STRING_EQ_PRINT_LENGTH];
	char locationLine[STRING_EQ_PRINT_LENGTH];

	if (left == nullptr || right == nullptr)
	{
		locationLine[0] = '^';
		locationLine[1] = 0;
		if (left == nullptr)
		{
			strcpy(leftLine, "nullptr");
			locCopyStringWithElipse(rightLine, right);
		}
		else
		{
			locCopyStringWithElipse(leftLine, left);
			strcpy(rightLine, "nullptr");
		}
	}
	else
	{
		char const* testLeft = left;
		char const* testRight = right;

		int offset = 0;
		for (; *testLeft && *testRight; ++offset, ++testLeft, ++testRight)
		{
			if (*testLeft != *testRight)
				break;
		}

		// reached the end of both strings, so they're the same
		if (!*testLeft && !*testRight)
			return true;

		locCopyStringWithElipse(leftLine, left, offset);
		locCopyStringWithElipse(rightLine, right, offset);

		if (offset > STRING_EQ_PRINT_LENGTH / 2)
			offset = STRING_EQ_PRINT_LENGTH / 2;

		memset(locationLine, ' ', offset);
		locationLine[offset] = '^';
		locationLine[offset + 1] = 0;
	}

	AddError();
	LogMessage(prefix, condition, leftLine, locationLine, rightLine);
	return false;
}
//---------------------------------------------------------------------------------
// Write error into current error object and advance pointer if there's still enough space
//---------------------------------------------------------------------------------
void TestFixture::LogMessage(char const* string, ...)
{
	uintptr_t spaceLeft = (myMessageSpace + MESSAGE_SPACE) - (char*)myNextError;

	if (spaceLeft == 0)
	{
		return;
	}

	spaceLeft -= sizeof(TestError);

	va_list args;

	va_start(args, string);

	int printedChars = vsnprintf(myNextError->message, spaceLeft, string, args);

	va_end(args);

	// if there isn't a reasonable amount of space left then just advance to end and stop printing errors
	if (printedChars < (int)(spaceLeft - sizeof(TestError) - 64))
	{
		uintptr_t nextOffset = (uintptr_t(myNextError->message) + printedChars + sizeof(TestError) * 2 - 1);
		nextOffset -= nextOffset % alignof(TestError);
		TestError* next = (TestError*)(nextOffset);
		myNextError->next = next;
	}
	else
	{
		myNextError->next = (TestError*)(myMessageSpace + MESSAGE_SPACE);
	}

	myNextError = myNextError->next;
}
TestFixture const* TestFixture::LinkTest(TestFixture* test)
{
	test->myNextTest = TestFixture::ourFirstTest;
	TestFixture::ourFirstTest = test;
	return test;
}

//---------------------------------------------------------------------------------
// Standard / Example runners
//---------------------------------------------------------------------------------
static bool locExecuteTest(TestFixture* test, TestFixture::OutputMode output)
{
	clock_t start = 0;
	if (output == TestFixture::Verbose)
	{
        std::cout << std::format("Running [{}/{}]",test->TestGroup(), test->TestName());
		start = clock();
	}

	if (test->ExecuteTest())
	{
		if (output == TestFixture::Verbose)
		{
			clock_t end = clock();
            std::cout << std::format(": Passed {} out of {} tests in {} seconds", test->NumTests(), test->NumTests(), float(end - start) / (float)CLOCKS_PER_SEC) << std::endl;
		}
		return true;
	}

	if (output != TestFixture::Silent)
	{
		if (output != TestFixture::Verbose)
            std::cout << std::format("[{}/{}]", test->TestGroup(), test->TestName());

        std::cout << std::format(": Failed {} out of {} tests", test->NumErrors(), test->NumTests());

		for (TestError const* err = test->GetFirstError(), *e = test->GetLastError(); err != e; err = err->next)
		{
            std::cout << std::format("{}", err->message) << std::endl;
		}
	}

	return false;
}

bool TestFixture::ExecuteAllTests(char const* groupFilter, char const* nameFilter, OutputMode output)
{
	if (output != Silent)
	{
		if (groupFilter == nullptr && nameFilter == nullptr)
            std::cout << "Running all tests." << std::endl;
		else if (groupFilter != nullptr && nameFilter == nullptr)
            std::cout << std::format("Running all tests in groups [{}].", groupFilter) << std::endl;
		else if (groupFilter == nullptr && nameFilter != nullptr)
            std::cout << std::format("Running all tests named [{}].", nameFilter) << std::endl;
		else
            std::cout << std::format("Running all tests named [{}/{}].", groupFilter, nameFilter) << std::endl;
	}

	int count = 0;
	int passes = 0;
	int fails = 0;
	bool passed = true;
	for (auto i = TestFixture::GetFirstTest(); i; i = i->GetNextTest())
	{
		bool matchGroup = groupFilter == nullptr || strcmp(groupFilter, i->TestGroup()) == 0;
		bool matchName = nameFilter == nullptr || strcmp(nameFilter, i->TestName()) == 0;
		if (matchGroup && matchName)
		{
			++count;
			passed &= locExecuteTest(i, output);
			passes += i->NumTests();
			fails += i->NumErrors();
		}
	}

	if (output != Silent)
	{
		if (count == 0)
            std::cout << "Failed to find any tests." << std::endl;
		else if (passed)
            std::cout << std::format("{} Tests finished. All {} assertions are passing.", count, passes) << std::endl;
		else
            std::cout << std::format("{} Tests finished, {} of {} assertions failed. Some tests are reporting errors.", count, fails, passes) << std::endl;
	}
	return passed;
}
