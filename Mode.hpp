#ifndef HPP_MODE
#define HPP_MODE

#include <string>
#include <vector>

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

// Multi-pattern (--matching-list) limits. PATTERN_NIBBLES also passed to the
// kernel via -D so host and device agree on the per-pattern stride.
#define PROFANITY_MAX_PATTERNS 64
#define PROFANITY_PATTERN_NIBBLES 16

enum HashTarget {
	ADDRESS,
	CONTRACT,
	HASH_TARGET_COUNT
};

class Mode {
	private:
		Mode();

	public:
		static Mode matching(const std::string strHex);
		static Mode matchingList(const std::vector<std::string> & words);
		static Mode range(const cl_uchar min, const cl_uchar max);
		static Mode leading(const char charLeading);
		static Mode leadingRange(const cl_uchar min, const cl_uchar max);
		static Mode mirror();

		static Mode benchmark();
		static Mode zeros();
		static Mode zeroBytes();
		static Mode letters();
		static Mode numbers();
		static Mode doubles();

		std::string name;

		std::string kernel;

		HashTarget target;
		// kernel transform fn name
		std::string transformKernel() const;
		// Address, Contract, ...
		std::string transformName() const;

		cl_uchar data1[20];
		cl_uchar data2[20];
		cl_uchar score;

		// Multi-pattern mode (--matching-list). patNibbles is patCount rows of
		// PROFANITY_PATTERN_NIBBLES nibbles (0-15); patLen[p] is row p's length.
		bool multipattern = false;
		cl_uchar patCount = 0;
		std::vector<cl_uchar> patNibbles;
		std::vector<cl_uchar> patLen;
};

#endif /* HPP_MODE */
