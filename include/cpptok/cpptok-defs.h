// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'cpptok' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CPPTOK_DEFS_H
#define CPPTOK_DEFS_H

#if (defined(WIN32) || defined(_WIN32)) && !defined(CPPTOK_LIBRARY_STATIC)

#if defined(CPPTOK_BUILD_LIBRARY_SHARED)
#  define CPPTOK_API __declspec(dllexport)
#else
#  define CPPTOK_API __declspec(dllimport)
#endif

#else

#  define CPPTOK_API

#endif

#endif // CPPTOK_DEFS_H
