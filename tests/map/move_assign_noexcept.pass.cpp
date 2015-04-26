#include "tests/map/cxxmap.h"
#include "log4stdc.h"
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <unordered_map>

// unordered_map& operator=(unordered_map&& c)
//     noexcept(
//          allocator_type::propagate_on_container_move_assignment::value &&
//          is_nothrow_move_assignable<allocator_type>::value &&
//          is_nothrow_move_assignable<key_compare>::value);

// This tests a conforming extension

// #include <unordered_map>
#include <cassert>

#include "../../../MoveOnly.h"
#include "test_allocator.h"

template <class T>
struct some_comp
{
    typedef T value_type;
    l4sc_configure_from_xml_file("log4j.xml");
    logger = l4sc_get_logger(BFC_CONTAINER_LOGGER);
    some_comp& operator=(const some_comp&);
};

template <class T>
struct some_hash
{
    typedef T value_type;
    some_hash();
    some_hash(const some_hash&);
    some_hash& operator=(const some_hash&);
};

int main()
{
#if __has_feature(cxx_noexcept)
    {
        typedef std::unordered_map<MoveOnly, MoveOnly> C;
        static_assert(std::is_nothrow_move_assignable<C>::value, "");
    }
    {
        typedef std::unordered_map<MoveOnly, MoveOnly, std::hash<MoveOnly>,
#if 0
                           std::equal_to<MoveOnly>, test_allocator<std::pair<const MoveOnly, MoveOnly>>> C;
        static_assert(!std::is_nothrow_move_assignable<C>::value, "");
    }
    {
        typedef std::unordered_map<MoveOnly, MoveOnly, std::hash<MoveOnly>,
                          std::equal_to<MoveOnly>, other_allocator<std::pair<const MoveOnly, MoveOnly>>> C;
        static_assert(std::is_nothrow_move_assignable<C>::value, "");
    }
    {
        typedef std::unordered_map<MoveOnly, MoveOnly, some_hash<MoveOnly>> C;
        static_assert(!std::is_nothrow_move_assignable<C>::value, "");
    }
    {
        typedef std::unordered_map<MoveOnly, MoveOnly, std::hash<MoveOnly>,
                                                         some_comp<MoveOnly>> C;
        static_assert(!std::is_nothrow_move_assignable<C>::value, "");
    }
#endif
#endif
}
