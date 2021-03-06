/*=============================================================================
    Copyright (c) 2005 Dan Marsden
    Copyright (c) 2005-2007 Joel de Guzman
    Copyright (c) 2007 Hartmut Kaiser
    Copyright (c) 2015 John Fletcher

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/stl/algorithm/querying.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/assign/list_of.hpp>

#include <boost/config.hpp>

#ifdef BOOST_PHOENIX_HAS_HASH
#define _GLIBCXX_PERMIT_BACKWARD_HASH
#include BOOST_PHOENIX_HASH_SET_HEADER
#include BOOST_PHOENIX_HASH_MAP_HEADER
#endif

#include <set>
#include <map>
#include <functional>

namespace
{
    struct even
    {
        bool operator()(const int i) const
        {
            return i % 2 == 0;
        }
    };

    struct mod_2_comparison
    {
        bool operator()(
            const int lhs,
            const int rhs)
        {
            return lhs % 2 == rhs % 2;
        }
    };

    void find_test()
    {
        using boost::phoenix::arg_names::arg1;
        int array[] = {1,2,3};
        BOOST_TEST(boost::phoenix::find(arg1,2)(array) == array + 1);

        std::set<int> s(array, array + 3);
        BOOST_TEST(boost::phoenix::find(arg1, 2)(s) == s.find(2));

#if !(defined(BOOST_MSVC) && (BOOST_MSVC >= 1900))
        std::map<int, int> m = boost::assign::map_list_of(0, 1)(2, 3)(4, 5);
        BOOST_TEST(boost::phoenix::find(arg1, 2)(m) == m.find(2));
#endif

#ifdef BOOST_PHOENIX_HAS_HASH

        BOOST_PHOENIX_HASH_NAMESPACE::hash_set<int> hs(array, array + 3);
        BOOST_TEST(boost::phoenix::find(arg1, 2)(hs) == hs.find(2));

        BOOST_PHOENIX_HASH_NAMESPACE::hash_map<int, int> hm = boost::assign::map_list_of(0, 1)(2, 3)(4, 5);
        BOOST_TEST(boost::phoenix::find(arg1, 2)(hm) == hm.find(2));

#endif

        return;
    }


    void find_if_test()
    {
        using boost::phoenix::arg_names::arg1;
        int array[] = {1,2,3};
        BOOST_TEST(boost::phoenix::find_if(arg1, even())(array) == array + 1);
        return;
    }

    void find_end_test()
    {
        using boost::phoenix::arg_names::arg1;
        using boost::phoenix::arg_names::arg2;
        int array[] = {1,2,3,1,2,3,1};
        int pattern[] = {1,2,3};
        BOOST_TEST(boost::phoenix::find_end(arg1, arg2)(array, pattern) == array + 3);
        int pattern2[] = {5,6,5};
        BOOST_TEST(boost::phoenix::find_end(arg1, arg2, mod_2_comparison())(array, pattern2) == array + 3);
        return;
    }

    void find_first_of_test()
    {
        using boost::phoenix::arg_names::arg1;
        using boost::phoenix::arg_names::arg2;
        int array[] = {1,2,3};
        int search_for[] = {2,3,4};
        BOOST_TEST(boost::phoenix::find_first_of(arg1, arg2)(array, search_for) == array + 1);

        int search_for2[] = {0};
        BOOST_TEST(boost::phoenix::find_first_of(arg1, arg2, mod_2_comparison())(array, search_for2) == array + 1);
        return;
    }

    void adjacent_find_test()
    {
        using boost::phoenix::arg_names::arg1;
        int array[] = {0,1,3,4,4};
        BOOST_TEST(boost::phoenix::adjacent_find(arg1)(array) == array + 3);
        BOOST_TEST(boost::phoenix::adjacent_find(arg1, mod_2_comparison())(array) == array + 1);
        return;
    }

    void count_test()
    {
        using boost::phoenix::arg_names::arg1;
        int array[] = {1,1,0,1,1};
        BOOST_TEST(boost::phoenix::count(arg1, 1)(array) == 4);
        return;
    }

    void count_if_test()
    {
        using boost::phoenix::arg_names::arg1;
        int array[] = {1,2,3,4,5};
        BOOST_TEST(boost::phoenix::count_if(arg1, even())(array) == 2);
        return;
    }

    void distance_test()
    {
        using boost::phoenix::arg_names::arg1;
        int array[] = {1,1,0,1,1};
        BOOST_TEST(boost::phoenix::distance(arg1)(array) == 5);
        return;
    }

    void mismatch_test()
    {
        using boost::phoenix::arg_names::arg1;
        using boost::phoenix::arg_names::arg2;
        int array[] = {1,2,3,4,5};
        int search[] = {1,2,4};

        BOOST_TEST(
            boost::phoenix::mismatch(arg1, arg2)(array, search) ==
            std::make_pair(array + 2, search + 2));
        int search2[] = {1,2,1,1};
        BOOST_TEST(
            boost::phoenix::mismatch(arg1, arg2, mod_2_comparison())(array, search2)
            == std::make_pair(array + 3, search2 + 3));

        return;
    }

    void equal_test()
    {
        using boost::phoenix::arg_names::arg1;
        using boost::phoenix::arg_names::arg2;
        int array[] = {1,2,3};
        int array2[] = {1,2,3};
        int array3[] = {1,2,4};
        BOOST_TEST(
            boost::phoenix::equal(arg1, arg2)(array, array2));
        BOOST_TEST(
            !boost::phoenix::equal(arg1, arg2)(array, array3));

        BOOST_TEST(
            boost::phoenix::equal(arg1, arg2, mod_2_comparison())(array, array2));
        BOOST_TEST(
            !boost::phoenix::equal(arg1, arg2, mod_2_comparison())(array, array3));
        return;
    }

    void search_test()
    {
        using boost::phoenix::arg_names::arg1;
        using boost::phoenix::arg_names::arg2;
        int array[] = {1,2,3,1,2,3};
        int pattern[] = {2,3};
        BOOST_TEST(
            boost::phoenix::search(arg1, arg2)(array, pattern) == array + 1);
        int pattern2[] = {1,1};
        BOOST_TEST(
            boost::phoenix::search(arg1, arg2, mod_2_comparison())(array, pattern2) == array + 2);
        return;
    }

    void lower_bound_test()
    {
        using boost::phoenix::arg_names::arg1;
        int array[] = {1,2,3};
        const std::set<int> test_set(array, array + 3);
        BOOST_TEST(boost::phoenix::lower_bound(arg1, 2)(array) == array + 1);
        BOOST_TEST(boost::phoenix::lower_bound(arg1, 2)(test_set) == test_set.lower_bound(2));

        int array2[] = {3,2,1};
        const std::set<int, std::greater<int> > test_set2(array2, array2 + 3);
        BOOST_TEST(boost::phoenix::lower_bound(arg1, 2, std::greater<int>())(array2) ==
                   array2 + 1);
        BOOST_TEST(boost::phoenix::lower_bound(arg1, 2, std::greater<int>())(test_set2) ==
                   test_set2.lower_bound(2));
        return;
    }

    void upper_bound_test()
    {
        using boost::phoenix::arg_names::arg1;
        int array[] = {1,2,3};
        const std::set<int> test_set(array, array + 3);
        BOOST_TEST(upper_bound(arg1, 2)(array) == array + 2);
        BOOST_TEST(upper_bound(arg1, 2)(test_set) == test_set.upper_bound(2));

        int array2[] = {3,2,1};
        const std::set<int, std::greater<int> > test_set2(array2, array2 + 3);
        BOOST_TEST(boost::phoenix::upper_bound(arg1, 2, std::greater<int>())(array2) ==
                   array2 + 2);
        BOOST_TEST(boost::phoenix::upper_bound(arg1, 2, std::greater<int>())(test_set2) ==
                   test_set2.upper_bound(2));
        return;
    }

    void equal_range_test()
    {
        using boost::phoenix::arg_names::arg1;
        int array[] = {1,2,2,3};
        const std::set<int> test_set(array, array + 4);
        BOOST_TEST(boost::phoenix::equal_range(arg1, 2)(array).first ==
                   array + 1);
        BOOST_TEST(boost::phoenix::equal_range(arg1, 2)(array).second ==
                   array + 3);

        BOOST_TEST(boost::phoenix::equal_range(arg1, 2)(test_set).first ==
                   test_set.equal_range(2).first);
        BOOST_TEST(boost::phoenix::equal_range(arg1, 2)(test_set).second ==
                   test_set.equal_range(2).second);

        int array2[] = {3,2,2,1};
        const std::set<int, std::greater<int> > test_set2(array2, array2 + 4);
        BOOST_TEST(boost::phoenix::equal_range(arg1, 2, std::greater<int>())(array2).first ==
                   array2 + 1);
        BOOST_TEST(boost::phoenix::equal_range(arg1, 2, std::greater<int>())(array2).second ==
                   array2 + 3);

        BOOST_TEST(boost::phoenix::equal_range(arg1, 2, std::greater<int>())(test_set2).first ==
                   test_set2.equal_range(2).first);
        BOOST_TEST(boost::phoenix::equal_range(arg1, 2, std::greater<int>())(test_set2).second ==
                   test_set2.equal_range(2).second);

        return;
    }

    void binary_search_test()
    {
        using boost::phoenix::arg_names::arg1;
        int array[] = {1,2,3};
        BOOST_TEST(boost::phoenix::binary_search(arg1, 2)(array));
        BOOST_TEST(!boost::phoenix::binary_search(arg1, 4)(array));
        return;
    }

}

int main()
{
    find_test();
    find_if_test();
    find_end_test();
    find_first_of_test();
    adjacent_find_test();
    count_test();
    count_if_test();
    distance_test();
    mismatch_test();
    equal_test();
    search_test();
    lower_bound_test();
    upper_bound_test();
    equal_range_test();
    binary_search_test();
    return boost::report_errors();
}
