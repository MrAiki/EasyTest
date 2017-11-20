#include "test.h"

/* 各テストスイートのセットアップ関数宣言 */
void testExample_Setup(void);

int main(int argc, char **argv)
{
  int ret;

  Test_Initialize();

  testExample_Setup();

  ret = Test_RunAllTestSuite();

  Test_Finalize();

  return ret;
}
