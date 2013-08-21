
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-
/// cppkit - http://www.cppkit.org
/// Copyright (c) 2013, Tony Di Croce
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without modification, are permitted
/// provided that the following conditions are met:
///
/// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
///    the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
///    and the following disclaimer in the documentation and/or other materials provided with the
///    distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
/// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
/// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
/// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
/// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// The views and conclusions contained in the software and documentation are those of the authors and
/// should not be interpreted as representing official policies, either expressed or implied, of the cppkit
/// Project.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=--=-=-=-=-=-

#ifndef cppkit_os_exports_h
#define cppkit_os_exports_h

#include "cppkit/ck_types.h"

// Do we have both exporting and importing macros defined?
#if (defined(CK_API_EXPORT) && defined(CK_API_IMPORT))
  // Report an error - it does not make sense to define both macros
  #error Cannot define both CK_API_EXPORT and CK_API_IMPORT
#endif

// Make sure the macros set the thhis header are cleared
#undef CK_API
#undef DLLIMPORT
#undef DLLEXPORT

// Is this being compiled for Windows?
#if (defined(IS_WINDOWS))
  // Is the module configured to default to export
  // mode or are we temporarily switching to export mode?
  #if (defined(CK_API_EXPORT) && !defined(_CK_API_IGNORE_DEFAULT_)) || defined(_CK_API_SWITCH_TO_EXPORT_)
    // Setup to export all X_API functions/methods
    #define CK_API __declspec(dllexport)
  // Is the module configured to default to import mode or are we temporarily switching to import mode?
  #elif (defined(CK_API_IMPORT) && !defined(_CK_API_IGNORE_DEFAULT_)) || defined(_CK_API_SWITCH_TO_IMPORT_)
    // Setup to import all CK_API functions/methods
    #define CK_API __declspec(dllimport)
  #endif

  // Define some import/export macros that will
  // not be modified by the cppkit specific macros
  #ifndef DLLIMPORT
    #define DLLIMPORT __declspec(dllimport)
  #endif
  #ifndef DLLEXPORT
    #define DLLEXPORT __declspec(dllexport)
  #endif
#endif

// On *nix, CK_API et al. expand into nothingness...

#if (!defined(CK_API))
  #define CK_API
#endif

#if (!defined(DLLIMPORT))
  #define DLLIMPORT
#endif

#if (!defined(DLLEXPORT))
  #define DLLEXPORT
#endif

#endif
