#define BOOST_TEST_MODULE RadProtoDictionariesTests

#include "radproto/dictionaries.h"
#include "radproto/error.h"
#include <string>
#include <map>
#include <cstdint> //uint8_t, uint32_t
#include <stdexcept>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wparentheses"
#include <boost/test/unit_test.hpp>
#pragma GCC diagnostic pop

BOOST_AUTO_TEST_SUITE(DictionariesTests)

BOOST_AUTO_TEST_CASE(FunctionAdd)
{
    RadProto::BasicDictionary b;

    b.add(1, "User-Name");
    BOOST_CHECK_THROW(b.add(1, "User"), RadProto::Exception);
    b.add(2, "abc");
    BOOST_CHECK_THROW(b.add(3, "abc"), RadProto::Exception);

    BOOST_CHECK_EQUAL(b.name(1), "User-Name");
    BOOST_CHECK_EQUAL(b.name(2), "abc");
    BOOST_CHECK_EQUAL(b.code("User-Name"), 1);
    BOOST_CHECK_EQUAL(b.code("abc"), 2);

    BOOST_CHECK_THROW(b.name(3), std::out_of_range);

    BOOST_CHECK_THROW(b.code("User"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(FunctionAppend)
{
    RadProto::BasicDictionary a;

    a.add(2, "def");
    a.add(3, "ghi");
    a.add(4, "User-Name");

    BOOST_CHECK_EQUAL(a.name(2), "def");
    BOOST_CHECK_EQUAL(a.name(3), "ghi");
    BOOST_CHECK_EQUAL(a.name(4), "User-Name");
    BOOST_CHECK_EQUAL(a.code("def"), 2);
    BOOST_CHECK_EQUAL(a.code("ghi"), 3);
    BOOST_CHECK_EQUAL(a.code("User-Name"), 4);

    RadProto::BasicDictionary b;

    b.add(1, "User-Name");

    BOOST_CHECK_EQUAL(b.name(1), "User-Name");
    BOOST_CHECK_EQUAL(b.code("User-Name"), 1);
    BOOST_CHECK_THROW(a.append(b), RadProto::Exception);

    RadProto::BasicDictionary c;

    c.add(4, "User");

    BOOST_CHECK_EQUAL(c.name(4), "User");
    BOOST_CHECK_EQUAL(c.code("User"), 4);
    BOOST_CHECK_THROW(a.append(c), RadProto::Exception);

    RadProto::BasicDictionary d;

    d.add(5, "jkl");

    BOOST_CHECK_EQUAL(d.name(5), "jkl");
    BOOST_CHECK_EQUAL(d.code("jkl"), 5);

    a.append(d);
}

BOOST_AUTO_TEST_CASE(BasicDictionary)
{
    RadProto::BasicDictionary b;

    BOOST_CHECK_THROW(b.name(0), std::out_of_range);

    BOOST_CHECK_THROW(b.code(""), std::out_of_range);

    b.add(1, "User-Name");
    BOOST_CHECK_THROW(b.add(1, "User"), RadProto::Exception);
    b.add(5, "ijk");

    BOOST_CHECK_EQUAL(b.name(1), "User-Name");
    BOOST_CHECK_EQUAL(b.name(5), "ijk");
    BOOST_CHECK_EQUAL(b.code("User-Name"), 1);
    BOOST_CHECK_EQUAL(b.code("ijk"), 5);
    BOOST_CHECK_THROW(b.code("User"), std::out_of_range);

    RadProto::BasicDictionary c;
    c.add(2, "def");
    c.add(3, "ghi");

    BOOST_CHECK_EQUAL(c.name(2), "def");
    BOOST_CHECK_EQUAL(c.name(3), "ghi");
    BOOST_CHECK_EQUAL(c.code("def"), 2);
    BOOST_CHECK_EQUAL(c.code("ghi"), 3);

    c.append(b);

    BOOST_CHECK_EQUAL(c.name(2), "def");
    BOOST_CHECK_EQUAL(c.name(3), "ghi");
    BOOST_CHECK_EQUAL(c.name(1), "User-Name");
    BOOST_CHECK_EQUAL(c.name(5), "ijk");
    BOOST_CHECK_EQUAL(c.code("def"), 2);
    BOOST_CHECK_EQUAL(c.code("ghi"), 3);
    BOOST_CHECK_EQUAL(c.code("User-Name"), 1);
    BOOST_CHECK_EQUAL(c.code("ijk"), 5);
}
BOOST_AUTO_TEST_SUITE_END()
