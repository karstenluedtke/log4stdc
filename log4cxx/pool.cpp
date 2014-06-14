
#include <log4cxx/logstring.h>
#include <log4cxx/helpers/pool.h>

using namespace log4cxx::helpers;
using namespace log4cxx;


Pool::Pool() : pool(0), release(true) {
}

Pool::Pool(apr_pool_t* p, bool release1) : pool(p), release(release1) {
}

Pool::~Pool() {
}


apr_pool_t* Pool::getAPRPool() {
   return pool;
}

apr_pool_t* Pool::create() {
    return pool;
}

void* Pool::palloc(size_t size) {
  return 0;
}

char* Pool::pstralloc(size_t size) {
  return 0;
}

char* Pool::itoa(int n) {
    return 0;
}

char* Pool::pstrndup(const char* s, size_t len) {
    return 0;
}

char* Pool::pstrdup(const char* s) {
    return 0;
}

char* Pool::pstrdup(const std::string& s) {
    return 0;
}
