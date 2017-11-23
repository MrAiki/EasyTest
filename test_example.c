#include "test.h"
#include <stddef.h> /* NULL */

/* cファイルを直接インクルード. */
/* ローカル変数もスコープに入るので、翻訳単位でのテストが可能 */
#include "example.c"

static int testExample_Initialize(void *obj)
{
  /* テストスイート共通の初期化処理 */
  TEST_UNUSED_PARAMETER(obj);
  return 0;
}

static int testExample_Finalize(void *obj)
{
  /* テストスイート共通の終了処理 */
  TEST_UNUSED_PARAMETER(obj);
  return 0;
}

static void testExample_CallApiTest(void *obj)
{
  TEST_UNUSED_PARAMETER(obj);

  /* my_exp(0)とexp(0)が等しいかテスト */
  Test_AssertCondition(my_exp(0) == exp(0));
  Test_AssertEqual(my_exp(0), exp(0));

  /* my_exp(0)とexp(1)が等しくないかテスト */
  Test_AssertNotEqual(my_exp(0), exp(1));

}

void testExample_Setup(void)
{
  struct TestSuite *suite
    = Test_AddTestSuite("Example Module Test Suite",
        NULL, testExample_Initialize, testExample_Finalize);
  Test_AddTest(suite, testExample_CallApiTest);
}
