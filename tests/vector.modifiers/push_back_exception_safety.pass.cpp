#include "tests/vector/cxxvector.h"
#include "log4stdc.h"
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// void push_back(const value_type& x);

#include <vector>
#include <cassert>

// Flag that makes the copy constructor for CMyClass throw an exception
static bool gCopyConstructorShouldThow = false;


class CMyClass {
    public: CMyClass(int tag);
    public: CMyClass(const CMyClass& iOther);
    public: ~CMyClass();

    bool equal(const CMyClass &rhs) const
        { return fTag == rhs.fTag && fMagicValue == rhs.fMagicValue; }
    private:
        int fMagicValue;
        int fTag;
        
    private: static int kStartedConstructionMagicValue;
    private: static int kFinishedConstructionMagicValue;
};

// Value for fMagicValue when the constructor has started running, but not yet finished
int CMyClass::kStartedConstructionMagicValue = 0;
// Value for fMagicValue when the constructor has finished running
int CMyClass::kFinishedConstructionMagicValue = 12345;

CMyClass::CMyClass(int tag) :
    fMagicValue(kStartedConstructionMagicValue), fTag(tag)
{
    // Signal that the constructor has finished running
    fMagicValue = kFinishedConstructionMagicValue;
}

CMyClass::CMyClass(const CMyClass& iOther) :
    fMagicValue(kStartedConstructionMagicValue), fTag(iOther.fTag)
{
    // If requested, throw an exception _before_ setting fMagicValue to kFinishedConstructionMagicValue
    if (gCopyConstructorShouldThow) {
        throw std::exception();
    }
    // Signal that the constructor has finished running
    fMagicValue = kFinishedConstructionMagicValue;
}

CMyClass::~CMyClass() {
    // Only instances for which the constructor has finished running should be destructed
    assert(fMagicValue == kFinishedConstructionMagicValue);
}

bool operator==(const CMyClass &lhs, const CMyClass &rhs) { return lhs.equal(rhs); }

int main()
{
    CMyClass instance(42);
    barefootc::vector<CMyClass> vec;

    vec.push_back(instance);
    barefootc::vector<CMyClass> vec2(vec);

    gCopyConstructorShouldThow = true;
    try {
        vec.push_back(instance);
    }
    catch (...) {
        assert(vec==vec2);
    }
}
