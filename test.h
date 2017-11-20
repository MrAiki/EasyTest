#ifndef _TEST_H_INCLUDED_
#define _TEST_H_INCLUDED_

/* 未使用引数警告回避マクロ */
#define TEST_UNUSED_PARAMETER(arg) \
  if (&(arg) == &(arg)) { ; }

/* テスト追加（関数名付き） */
#define Test_AddTest(suite, function) \
  Test_AddTestWithName(suite, #function, function)

/* 初期化/終了関数型 */
typedef int (*TestInitFinFunctionType)(void *obj);

/* テスト関数 */
typedef int (*TestFunctionType)(void *obj);

/* テストスイート構造体 */
struct TestSuite;

/* テストの初期化 */
void Test_Initialize(void);

/* テストの終了 */
void Test_Finalize(void);

/* テストスイートの作成 */
struct TestSuite* 
Test_AddTestSuite(const char *suite_name,
    void *obj,
    TestInitFinFunctionType init_func,
    TestInitFinFunctionType fin_func);

/* テストスイートにテストを追加 */
void Test_AddTestWithName(
    struct TestSuite* test_suite,
    const char *test_name, 
    TestFunctionType test_func);

/* 全テストスイートの実行 */
int Test_RunAllTestSuite(void);

#endif /* _TEST_H_INCLUDED_ */
