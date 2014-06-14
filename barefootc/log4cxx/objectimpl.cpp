/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <log4cxx/logstring.h>
#include <log4cxx/helpers/objectimpl.h>

using namespace log4cxx::helpers;

ObjectImpl::ObjectImpl() : ref( 0 )
{
}

ObjectImpl::~ObjectImpl()
{
}

void ObjectImpl::addRef() const
{
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64)
	if (sizeof(this->ref) == 8) {
		__asm__ __volatile__(
		"lock incq %0" :"=m" (this->ref) :"m"  (this->ref));
	} else {
		__asm__ __volatile__(
		"lock incl %0" :"=m" (this->ref) :"m"  (this->ref));
	}
#elif defined(__i386) || defined(__i386__)
	__asm__ __volatile__(
	"lock incl %0" :"=m" (this->ref) :"m"  (this->ref));
#else
	ref++;
#endif
}

void ObjectImpl::releaseRef() const
{
	unsigned char c;

#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64)
	if (sizeof(this->ref) == 8) {
		__asm__ __volatile__(
		"lock decq %0; sete %1" :"=m" (this->ref), "=qm" (c)
					:"m" (this->ref) : "memory");
	} else {
		__asm__ __volatile__(
		"lock decl %0; sete %1" :"=m" (this->ref), "=qm" (c)
					:"m" (this->ref) : "memory");
 	}
#elif defined(__i386) || defined(__i386__)
	__asm__ __volatile__(
	"lock decl %0; sete %1" :"=m" (this->ref), "=qm" (c)
				:"m" (this->ref) : "memory");
#else
	c = (--ref == 0);
#endif
  	if (c) {
		delete this;
	}
}
