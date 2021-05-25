#include <cstring>
#include <iostream>

#include <pqxx/transaction>

#include "test_helpers.hxx"


using namespace pqxx;


// Example program for libpqxx.  Test binary string functionality.
namespace
{
void test_062()
{
  connection conn;
  work tx{conn};

  std::string const TestStr{
    "Nasty\n\030Test\n\t String with \200\277 weird bytes "
    "\r\0 and Trailer\\\\\0"};

  tx.exec0("CREATE TEMP TABLE pqxxbin (binfield bytea)");

  std::string const Esc{tx.esc_raw(TestStr)},
    Chk{tx.esc_raw(
      reinterpret_cast<unsigned char const *>(TestStr.c_str()),
      strlen(TestStr.c_str()))};

  PQXX_CHECK_EQUAL(Chk, Esc, "Inconsistent results from esc_raw().");

  tx.exec0("INSERT INTO pqxxbin VALUES ('" + Esc + "')");

  result R{tx.exec("SELECT * from pqxxbin")};
  tx.exec0("DELETE FROM pqxxbin");

  auto const B{R.at(0).at(0).as<std::basic_string<std::byte>>()};

  PQXX_CHECK(not std::empty(B), "Binary string became empty in conversion.");

  PQXX_CHECK_EQUAL(
    std::size(B), std::size(TestStr), "Binary string was mangled.");

  std::basic_string<std::byte>::const_iterator c;
  std::basic_string<std::byte>::size_type i;
  for (i = 0, c = std::begin(B); i < std::size(B); ++i, ++c)
  {
    PQXX_CHECK(c != std::end(B), "Premature end to binary string.");

    char const x{TestStr.at(i)}, y{char(B.at(i))}, z{char(B.data()[i])};

    PQXX_CHECK_EQUAL(
      std::string(&x, 1), std::string(&y, 1), "Binary string byte changed.");

    PQXX_CHECK_EQUAL(
      std::string(&y, 1), std::string(&z, 1),
      "Inconsistent byte at offset " + to_string(i) + ".");
  }
}


PQXX_REGISTER_TEST(test_062);
} // namespace
