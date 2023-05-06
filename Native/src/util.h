#ifndef CATALYST_UTIL_H
#define CATALYST_UTIL_H

#define CATALYST_VERSION "1.0.0 compiled on " __DATE__ " at " __TIME__

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define LOG(msg) \
    printf("[%s: L%d] " msg, __FILENAME__, __LINE__)

#define LOGV(fmt, ...) \
    printf("[%s: L%d] " fmt, __FILENAME__, __LINE__, __VA_ARGS__)

#endif //CATALYST_UTIL_H
