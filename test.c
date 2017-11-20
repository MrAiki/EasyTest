#include <stdio.h>
#include <stdlib.h>

#include "test.h"

/* ログ出力（仮実装） */
#define Test_LogOutput(...) \
  fprintf(stdout, __VA_ARGS__)

/* テストスイートの実行 */
static int test_RunTestSuite(struct TestSuite *test_suite);

/* テスト関数リスト */
struct TestFuncList {
  struct TestFuncList *next;
  const char*         test_name;    /* テスト名   */
  TestFunctionType    test_func;    /* テスト関数 */
};

/* テストスイート */
struct TestSuite {
  const char*             suite_name;  /* スイート名 */
  void*                   obj;         /* 任意のオブジェクト */
  TestInitFinFunctionType init_func;   /* 初期化関数 */
  TestInitFinFunctionType fin_func;    /* 終了関数   */
  struct TestFuncList*    tests;       /* テスト関数リスト */
};

/* テストスイートのリスト */
struct TestSuiteList {
  struct TestSuiteList *next;
  struct TestSuite     suite;
};

/* テストスイートリストの実体 */
static struct TestSuiteList *st_test_suite_list;

/* テストの初期化 */
void Test_Initialize(void)
{
  st_test_suite_list = NULL;
}

/* テストの終了 */
void Test_Finalize(void)
{
  struct TestSuiteList *suite, *tmp_suite;
  struct TestFuncList *test, *tmp_test;

  /* 全スイートとテストを解放 */
  for (suite = st_test_suite_list;
       suite != NULL; ) {
    for (test = suite->suite.tests;
         test != NULL; ) {
      /* テストケースが一つもない */
      if (test == NULL) {
        break;
      }
      tmp_test = test->next;
      free(test);
      test = tmp_test;
    }
    tmp_suite = suite->next;
    free(suite);
    suite = tmp_suite;
  }
}

struct TestSuite* 
Test_AddTestSuite(const char *suite_name,
    void *obj,
    TestInitFinFunctionType init_func,
    TestInitFinFunctionType fin_func)
{
  struct TestSuiteList *list_pos;
  struct TestSuiteList *new_entry;

  /* 新しいスイートの作成 */
  new_entry 
    = (struct TestSuiteList *)malloc(sizeof(struct TestSuiteList));
  new_entry->next            = NULL;
  new_entry->suite.suite_name = suite_name;
  new_entry->suite.obj       = obj;
  new_entry->suite.init_func = init_func;
  new_entry->suite.fin_func  = fin_func;
  new_entry->suite.tests     = NULL;

  if (st_test_suite_list == NULL) {
    /* 初回のみ先頭に追加 */
    st_test_suite_list = new_entry;
  } else {
    /* 末尾に至るまでリストを辿る */
    for (list_pos        = st_test_suite_list;
         list_pos->next != NULL;
         list_pos        = list_pos->next) ;

    /* リストに追加 */
    list_pos->next = new_entry;
  }

  return &(new_entry->suite);
}

void Test_AddTestWithName(
    struct TestSuite* test_suite, 
    const char* test_name,
    TestFunctionType test_func)
{
  struct TestFuncList *list_pos;
  struct TestFuncList *new_entry;

  if (test_suite == NULL) {
    return;
  }

  /* 新しいテストの作成 */
  new_entry 
    = (struct TestFuncList *)malloc(sizeof(struct TestFuncList));
  new_entry->next       = NULL;
  new_entry->test_name  = test_name;
  new_entry->test_func  = test_func;

  /* リストに追加 */
  if (test_suite->tests == NULL) {
    /* 初回のみ先頭に追加 */
    test_suite->tests    = new_entry;
  } else {
    for (list_pos        = test_suite->tests;
         list_pos->next != NULL;
         list_pos        = list_pos->next) ;
    list_pos->next = new_entry;
  }

}

/* 全テストスイートの実行 */
int Test_RunAllTestSuite(void)
{
  int ret = 0, tmp_ret;
  struct TestSuiteList *suite;

  for (suite  = st_test_suite_list;
       suite != NULL;
       suite  = suite->next) {

    /* 異常終了の回数だけインクリメント */
    tmp_ret = test_RunTestSuite(&suite->suite); 
    if (tmp_ret != 0) {
      ret++;
    }

  }

  Test_LogOutput("[Failures:%d] Test Done. \n", ret);

  return ret;
}

/* 一つのテストスイートを実行するサブルーチン */
static int test_RunTestSuite(struct TestSuite *test_suite)
{
  int ret, tmp_ret;
  struct TestFuncList* func_list;

  Test_LogOutput("====== %-30s ======\n", test_suite->suite_name);

  ret = 0;
  for (func_list = test_suite->tests;
       func_list != NULL;
       func_list = func_list->next) {

    /* 初期化関数実行 */
    test_suite->init_func(test_suite->obj);
    /* テスト実行 */
    tmp_ret = func_list->test_func(test_suite->obj);
    if (tmp_ret != 0) { ret++; }
    /* 終了関数実行 */
    test_suite->fin_func(test_suite->obj);

    /* 結果表示 */
    Test_LogOutput("[%7s] Test %-30s\n",
        (tmp_ret == 0) ? "SUCCESS" : "FAILED",
        func_list->test_name);

  }

  Test_LogOutput("[Failures:%d] %-30s Done. \n", ret, test_suite->suite_name);

  return ret;
}
