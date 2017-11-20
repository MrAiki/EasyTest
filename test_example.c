#include "test.h"
#include <stddef.h> /* NULL */

/* cファイルを直接インクルード. */
/* ローカル変数もスコープに入るので、翻訳単位でのテストが可能 */
#include "example.c"

int testExample_Initialize(void *obj)
{
  /* テストスイート共通の初期化処理 */
  TEST_UNUSED_PARAMETER(obj);
  return 0;
}

int testExample_Finalize(void *obj)
{
  /* テストスイート共通の終了処理 */
  TEST_UNUSED_PARAMETER(obj);
  return 0;
}

int testExample_CallApiTest(void *obj)
{
  TEST_UNUSED_PARAMETER(obj);

  if (my_exp(0) != exp(0)) {
    /* テスト失敗 */
    return 1;
  } else {
    /* テスト成功 */
    return 0;
  }

}

void testExample_Setup(void)
{
  struct TestSuite *suite
    = Test_AddTestSuite("Example Module Test Suite",
        NULL, testExample_Initialize, testExample_Finalize);
  Test_AddTest(suite, testExample_CallApiTest);
}
