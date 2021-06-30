/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License.

Module Name:

    Trace.h

Abstract:

    Header file for the debug tracing related function defintions and macros.

Environment:

    Kernel mode

--*/

//
// Define the tracing flags.
//
// Tracing GUID - 5fbb08b0-3d8d-4171-a1b8-5d48737110b7
//

#define WPP_CONTROL_GUIDS                                              \
    WPP_DEFINE_CONTROL_GUID(                                           \
        imxvpukmTraceGuid, (5fbb08b0,3d8d,4171,a1b8,5d48737110b7), \
                                                                            \
        WPP_DEFINE_BIT(IMXVPU_TRACING_DEFAULT)                         \
        WPP_DEFINE_BIT(IMXVPU_DRIVER_TRACE)                                   \
        WPP_DEFINE_BIT(IMXVPU_DEVICE_TRACE)                                   \
        WPP_DEFINE_BIT(IMXVPU_QUEUE_TRACE)                                    \
        )                             

#define WPP_FLAG_LEVEL_LOGGER(flag, level)                                  \
    WPP_LEVEL_LOGGER(flag)

#define WPP_FLAG_LEVEL_ENABLED(flag, level)                                 \
    (WPP_LEVEL_ENABLED(flag) &&                                             \
     WPP_CONTROL(WPP_BIT_ ## flag).Level >= level)

#define WPP_LEVEL_FLAGS_LOGGER(lvl,flags) \
           WPP_LEVEL_LOGGER(flags)
               
#define WPP_LEVEL_FLAGS_ENABLED(lvl, flags) \
           (WPP_LEVEL_ENABLED(flags) && WPP_CONTROL(WPP_BIT_ ## flags).Level >= lvl)

//           
// WPP orders static parameters before dynamic parameters. To support the Trace function
// defined below which sets FLAGS=MYDRIVER_ALL_INFO, a custom macro must be defined to
// reorder the arguments to what the .tpl configuration file expects.
//
#define WPP_RECORDER_FLAGS_LEVEL_ARGS(flags, lvl) WPP_RECORDER_LEVEL_FLAGS_ARGS(lvl, flags)
#define WPP_RECORDER_FLAGS_LEVEL_FILTER(flags, lvl) WPP_RECORDER_LEVEL_FLAGS_FILTER(lvl, flags)

//
// This comment block is scanned by the trace preprocessor to define our
// Trace function.
//
// begin_wpp config
// FUNC Trace{FLAGS=IMXVPU_TRACING_DEFAULT}(LEVEL, MSG, ...);
// FUNC TraceEvents(LEVEL, FLAGS, MSG, ...);
//
// FUNC IMXVPU_LOG_ERROR{LEVEL=TRACE_LEVEL_ERROR, FLAGS=IMXVPU_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (IMXVPU_LOG_ERROR, "%!STDPREFIX! [%s @ %u] ERROR :", __FILE__, __LINE__);
//
// FUNC IMXVPU_LOG_LOW_MEMORY{LEVEL=TRACE_LEVEL_ERROR, FLAGS=IMXVPU_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (IMXVPU_LOG_LOW_MEMORY, "%!STDPREFIX! [%s @ %u] LOW MEMORY :", __FILE__, __LINE__);
//
// FUNC IMXVPU_LOG_WARNING{LEVEL=TRACE_LEVEL_WARNING, FLAGS=IMXVPU_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (IMXVPU_LOG_WARNING, "%!STDPREFIX! [%s @ %u] WARNING :", __FILE__, __LINE__);
//
// FUNC IMXVPU_LOG_INFORMATION{LEVEL=TRACE_LEVEL_INFORMATION, FLAGS=IMXVPU_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (IMXVPU_LOG_INFORMATION, "%!STDPREFIX! [%s @ %u] INFO :", __FILE__, __LINE__);
//
// FUNC IMXVPU_LOG_TRACE{LEVEL=TRACE_LEVEL_VERBOSE, FLAGS=IMXVPU_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (IMXVPU_LOG_TRACE, "%!STDPREFIX! [%s @ %u] TRACE :", __FILE__, __LINE__);
//
// end_wpp
//
