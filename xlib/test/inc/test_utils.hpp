#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

# include <iostream>
# include <iomanip>

# define COUT std::cout
# define ENDL std::endl
# define COL_BLUE       "\033[0;34m"
# define COL_RED        "\033[0;31m"
# define COL_YELLOW     "\033[0;33m"
# define COL_MAGENTA    "\033[0;35m"
# define COL_DEFAULT    "\033[0m"

# define TESTER_HEADER(txt,col) COUT << col #txt COL_DEFAULT << ENDL
# define TESTER_SECTION_S(txt)  \
{\
COUT << "\n\n" << ENDL; \
TESTER_HEADER(=========================================, COL_MAGENTA); \
TESTER_HEADER(txt, COL_BLUE); \
TESTER_HEADER(-----------------------------------------, COL_MAGENTA); \
}

# define TESTER_SECTION_E()  \
TESTER_HEADER(=========================================, COL_MAGENTA) \

#endif  /*  TEST_UTILS_HPP */