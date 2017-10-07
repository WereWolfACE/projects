#ifndef __IOERROR_H__
#define __IOERROR_H__

#pragma once

#include <ErrorCodeEnum.h>

#define IO_ERROR_CODE_ENUM \
	(IOError, \
	((Success, "success")) \
	((InvalidArgument, "invalid argument")) \
	((ReadOnly, "read only")) \
	((EndOfStream, "unexpected end of stream")) \
	((FailedToOpen, "failed to open")) \
	((FailedToClose, "failed to close")) \
	((FailedToSeek, "failed to seek")) \
	((FailedToTell, "failed to find position")) \
	((FailedToRead, "failed to read")) \
	((FailedToWrite, "failed to write")) \
	((FailedToFlush, "failed to flush")))

DECLARE_ERROR_CODE_ENUM(IO_ERROR_CODE_ENUM)

#endif // __IOERROR_H__
