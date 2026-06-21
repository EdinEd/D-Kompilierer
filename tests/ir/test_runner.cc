#include <iostream>
#include <cstdlib>

extern "C" int test_add();
extern "C" int test_sub();
extern "C" int test_mul();
extern "C" int test_div();
extern "C" int test_mod();
extern "C" int test_eq_true();
extern "C" int test_eq_false();
extern "C" int test_neq();
extern "C" int test_less();
extern "C" int test_greater();
extern "C" int test_leq();
extern "C" int test_geq();
extern "C" int test_var_decl_init();
extern "C" int test_var_assign();
extern "C" int test_while();
extern "C" int test_if_then();
extern "C" int test_if_else();
extern "C" int test_else_if();
extern "C" int test_func_call();
extern "C" int test_nested_block();
extern "C" int test_complex_expr();
extern "C" int test_comments();

int main()
{
    int failed = 0;

#define RUN_TEST(name, expected) \
    do { \
        int result = name(); \
        std::cout << #name << ": expected=" << expected \
                  << " actual=" << result \
                  << (result == expected ? " PASS" : " FAIL") << std::endl; \
        if (result != expected) failed++; \
    } while(0)

    RUN_TEST(test_add, 3);
    RUN_TEST(test_sub, 2);
    RUN_TEST(test_mul, 12);
    RUN_TEST(test_div, 3);
    RUN_TEST(test_mod, 1);
    RUN_TEST(test_eq_true, 1);
    RUN_TEST(test_eq_false, 0);
    RUN_TEST(test_neq, 1);
    RUN_TEST(test_less, 1);
    RUN_TEST(test_greater, 1);
    RUN_TEST(test_leq, 1);
    RUN_TEST(test_geq, 1);
    RUN_TEST(test_var_decl_init, 42);
    RUN_TEST(test_var_assign, 99);
    RUN_TEST(test_while, 5);
    RUN_TEST(test_if_then, 1);
    RUN_TEST(test_if_else, 2);
    RUN_TEST(test_else_if, 3);
    RUN_TEST(test_func_call, 7);
    RUN_TEST(test_nested_block, 5);
    RUN_TEST(test_complex_expr, 15);
    RUN_TEST(test_comments, 0);

    if (failed > 0)
    {
        std::cout << std::endl << failed << " test(s) FAILED" << std::endl;
        return 1;
    }

    std::cout << std::endl << "All tests PASSED" << std::endl;
    return 0;
}
