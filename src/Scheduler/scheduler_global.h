


#ifdef _WINDOWS
#ifdef Scheduler_EXPORTS
#define SCHEDULER_EXPORT __declspec(dllexport)
#else
#define SCHEDULER_EXPORT __declspec(dllimport)
#endif
#else
#define SCHEDULER_EXPORT
#endif