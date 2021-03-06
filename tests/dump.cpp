
#include "../mmapio.hpp"
#include <cstdlib>
#include <iostream>
#include <limits>
#include <cctype>
#include <iomanip>
#include <system_error>

int main(int argc, char **argv) {
  mmapio::mmapio_i* mi;
  char const* fname;
  if (argc < 5) {
    std::cerr << "usage: dump (file) (mode) (offset) (length)" << std::endl;
    return EXIT_FAILURE;
  }
  fname = argv[1];
  try {
    mmapio::set_errno(0);
    mi = mmapio::open(fname, argv[2],
      (size_t)std::strtoul(argv[3],nullptr,0),
      (size_t)std::strtoul(argv[4],nullptr,0));
  } catch (std::exception const& e) {
    int err = mmapio::get_errno();
    std::cerr << "failed to map file '" << fname << "':" << std::endl;
    std::cerr << "\t" << e.what() << std::endl;
    std::cerr << "\t(errno = " << err << "; "
      << std::generic_category().message(err) << ")" << std::endl;
    return EXIT_FAILURE;
  }
  /* output the data */{
    size_t len = mi->length();
    unsigned char* bytes = (unsigned char*)mi->acquire();
    if (bytes != NULL) {
      size_t i;
      if (len >= std::numeric_limits<size_t>::max()-32)
        len = std::numeric_limits<size_t>::max()-32;
      for (i = 0; i < len; i+=16) {
        size_t j = 0;
        if (i)
          std::cout << std::endl;
        std::cout << std::setw(4) << std::setbase(16) << i << ':';
        for (j = 0; j < 16; ++j) {
          if (j%4 == 0) {
            std::cout << " ";
          }
          if (j < len-i)
            std::cout << std::setw(2) << std::setbase(16) << std::setfill('0')
              << (unsigned int)(bytes[i+j]);
          else std::cout << "  ";
        }
        std::cout << " | ";
        for (j = 0; j < 16; ++j) {
          if (j < len-i) {
            char ch = static_cast<char>(bytes[i+j]);
            std::cout << (isprint(ch) ? ch : '.');
          } else std::cout << ' ';
        }
      }
      std::cout << std::endl;
      mi->release(bytes);
    } else {
      std::cerr << "mapped file '" << fname <<
        "' gives no bytes?" << std::endl;
    }
  }
  delete mi;
  return EXIT_SUCCESS;
}

