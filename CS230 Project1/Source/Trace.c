//------------------------------------------------------------------------------
//
// File Name:	Trace.c
// Author(s):	Doug Schilling (dschilling)
// Project:		MyGame
// Course:		CS230S19
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>		// va_copy()

#include "Trace.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

// TODO: Declare a private variable for storing a file handle.
static FILE * file;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Tracing/Logging module.
void TraceInit()
{
  // Open "trace.log" for writing (text mode).
  // fopen_s:
  // https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=EN-US&k=k(stdio%2Ffopen_s);k(fopen_s);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
  if (fopen_s(&file, "trace.log", "wt") != 0)
  {
    // Error handling:
    // https://msdn.microsoft.com/en-us/library/9t0e6085.aspx
    perror("Error opening trace.log.");
  }
}

// Output a message to the Tracing/Logging file.
void TraceMessage(const char * formatString, ...)
{
  va_list arg_ptr;
  va_start(arg_ptr, formatString);
  // Write the message to the Tracing/Logging file.
  vfprintf_s(file, formatString, arg_ptr);
  fprintf_s(file, "\n");

  // Also print the message to the console window. 
  vprintf(formatString, arg_ptr);
  printf_s("\n");

  va_end(arg_ptr);
}

// Shutdown the Tracing/Logging module.
void TraceShutdown()
{
	// Close "trace.log" if it has been opened successfully.
  if (file != NULL)
  {
    fclose(file);
  }
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

