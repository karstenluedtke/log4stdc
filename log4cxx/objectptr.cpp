
#include <log4cxx/helpers/objectptr.h>

using namespace log4cxx::helpers;

ObjectPtrBase::ObjectPtrBase() {
}

ObjectPtrBase::~ObjectPtrBase() {
}

void ObjectPtrBase::checkNull(const int& null) {
}

void* ObjectPtrBase::exchange(void** destination, void* newValue) {
   void* oldValue = *destination;
   *destination = newValue;
   return oldValue;
}
