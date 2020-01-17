#ifndef _DEBUGGER_H_INCLUDED
#define _DEBUGGER_H_INCLUDED

#include <mutex>

static std::ostream _trash(NULL);

#ifdef DEBUG
#define plog std::cout << "LOG: " << __func__ << " (" << __LINE__ << "): "
#define perr std::cerr << "ERR: " << __func__ << " (" << __LINE__ << "): "
#else
#define plog _trash
#define perr std::cerr << "ERR: " << __func__ << " (" << __LINE__ << "): "
#endif


#endif // _DEBUGGER_H_INCLUDED
