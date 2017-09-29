#pragma once

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

struct IGCRef
{
	virtual int AddRef() = 0;
	virtual int GetRef() const = 0;
	virtual int Release() = 0;

	virtual void SetMark(int mark) = 0;
	virtual int GetMark() const = 0;
	virtual void gcBlacken() = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
