#ifndef MOD_ERROR_H
#define	MOD_ERROR_H

#include <iosfwd>
#include <string>
#include <vector>

// rst: This file contains the functinoality used for reporting errors.
// rst: In the description of the execeptions we use the standard terminology (see e.g., `Exception Safety <http://en.wikipedia.org/wiki/Exception_safety>`_).
// rst: If nothing else is specified a thrown exception from MØD provides no exception safety.
// rst: Any exception thrown from MØD or any dependencies are intented to derive from ``std::exception``,
// rst: and any exception from MØD derives from :class:`Exception`.
// rst: The exceptions are in general only thrown from the outer-most interface, and not from within the ``mod::lib`` namespace.

namespace mod {

// rst-class: Stacktrace
// rst:
// rst:		A class encapsulating the current stacktrace at construction time.
// rst:		Stacktraces may not be supported on all platforms.
// rst-class-start:

struct Stacktrace {
	// rst: .. function:: Stacktrace(unsigned int frameLimit, unsigned int numSkip)
	// rst:
	// rst:		Capture a stacktrace with at most ``frameLimit`` frames and without the first ``numSkip`` frames.
	// rst:		The frame from the constructor it self is always skipped.
	Stacktrace(unsigned int frameLimit, unsigned int numSkip);
	// rst: .. function:: void print(unsigned int frameLimit, std::ostream &s) const
	// rst: 
	// rst:		Print at most the first ``frameLimit`` frames from the capture stacktrace. Use 0 to print all available frames.
	// rst:		Symbol names will be demangled if possible.
	void print(unsigned int frameLimit, std::ostream &s) const;
private:
	std::vector<std::string> raw;
	std::vector<std::string> demangled;
};
// rst-class-end:


// rst-class: Exception : public std::exception
// rst:
// rst:		The base class of all MØD exceptions.
// rst-class-start:

struct Exception : public std::exception {
protected:

	Exception(std::string &&text, unsigned int numSkip, unsigned int frameLimit) : text(text), stacktrace(frameLimit, numSkip) { }

	Exception(std::string &&text, unsigned int numSkip) : Exception(std::move(text), numSkip, 20) { }

	Exception(std::string &&text) : Exception(std::move(text), 0) { }

public:
	// rst: .. function:: virtual std::string getName() const = 0
	// rst:
	// rst:		:returns: the name of the exception.
	virtual std::string getName() const = 0;
	// rst: .. function:: virtual const char *what() const noexcept
	// rst: 
	// rst:		:returns: the description for the exception.
	virtual const char *what() const noexcept;
	// rst: .. function:: void printStacktrace(unsigned int frameLimit, std::ostream &s) const
	// rst:
	// rst:		Print the stacktrace captured when the exception was constructed. See :func:`Stacktrace::print`.
	void printStacktrace(unsigned int frameLimit, std::ostream &s) const;
protected:
	std::string text;
	Stacktrace stacktrace;
	mutable std::string whatString; // is set by the what function
};
// rst-class-end:

// rst-class: FatalError : public Exception
// rst:
// rst:		When thrown there is no exception safety. It is not safe to continue after catching it.
// rst:
// rst-class-start:

struct FatalError : public Exception {

	FatalError(std::string &&text, unsigned int numSkip) : Exception(std::move(text), numSkip) { }

	FatalError(std::string &&text) : Exception(std::move(text)) { }

	std::string getName() const {
		return "MØD FatalError";
	}
	const char *what() const noexcept;
};
// rst-class-end:

// rst-class: InputError : public Exception
// rst:
// rst:		When thrown there is strong exception safety.
// rst:		This exception is thrown when bad data has been provided to a loading function.
// rst:
// rst-class-start:

struct InputError : public Exception {

	InputError(std::string &&text) : Exception(std::move(text)) { }

	std::string getName() const {
		return "MØD InputError";
	}
};
// rst-class-end:

// rst-class: LogicError : public Exception
// rst:
// rst:		When thrown there is strong exception safety.
// rst:		This exception is thrown when a pre-condition of a function is violated.
// rst:
// rst-class-start:

struct LogicError : public Exception {

	LogicError(std::string &&text) : Exception(std::move(text)) { }

	std::string getName() const {
		return "MØD LogicError";
	}
};
// rst-class-end:

void fatal(std::string function, std::string file, std::size_t line) __attribute__((__noreturn__)); // TODO: change to C++11 syntax at some point
#define MOD_ABORT fatal(__func__, __FILE__, __LINE__)

} // namespace mod

#endif	/* MOD_ERROR_H */
