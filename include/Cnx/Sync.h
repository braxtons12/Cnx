/// @file Sync.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief `Sync` provides higher-level synchronization abstractions and utilites to make
/// it easy to write multi-threaded code simply and correctly
/// @version 0.2.0
/// @date 2022-05-08
///
/// MIT License
/// @copyright Copyright (c) 2022 Braxton Salyer <braxtonsalyer@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

/// @defgroup cnx_sync Synchronization
/// Cnx provides various higher-level synchronization abstractions through various types and
/// utilities to make it easy to write multi-threaded code simply and correctly

#include <Cnx/sync/Condvar.h>
#include <Cnx/sync/Mutex.h>
#include <Cnx/sync/SharedLock.h>
#include <Cnx/sync/SharedMutex.h>
#include <Cnx/sync/UniqueLock.h>
