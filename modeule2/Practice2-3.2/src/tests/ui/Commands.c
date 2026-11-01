#include <ui/Commands.h>
#include <ui/InputArgs.h>

#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

// ---- cmdInitCount ----

void testCmdInitCount(void)
{
    InputArgs args = iaInit();

    TEST_ASSERT_EQUAL_INT(cmdInitCount("100", &args), ArgsParseResultOk);
    TEST_ASSERT_TRUE(args.countInputed);
    TEST_ASSERT_EQUAL_UINT32(args.count, 100);

    // Двойное определение
    TEST_ASSERT_EQUAL_INT(cmdInitCount("200", &args), ArgsParseResultDoubleInit);
    // Не число
    InputArgs a2 = iaInit();
    TEST_ASSERT_EQUAL_INT(cmdInitCount("abc", &a2), ArgsParseResultInvalidArg);
    // Нулевой контекст
    TEST_ASSERT_EQUAL_INT(cmdInitCount("100", NULL), ArgsParseResultDevError);
}

// ---- cmdInitMask ----
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

void testCmdInitMaskFromIp(void)
{
    InputArgs args = iaInit();
    TEST_ASSERT_EQUAL_INT(cmdInitMask("255.255.255.0", &args), ArgsParseResultOk);
    TEST_ASSERT_TRUE(args.maskInputed);
    printf("%u\n", args.mask);
    printf("%u\n", 0xFFFFFF00u);
    TEST_ASSERT_EQUAL_UINT32(args.mask, 0xFFFFFF00u);

    // Двойное определение
    TEST_ASSERT_EQUAL_INT(cmdInitMask("255.255.255.0", &args), ArgsParseResultDoubleInit);
}

void testCmdInitMaskFromUInt32(void)
{
    InputArgs args = iaInit();

    TEST_ASSERT_EQUAL_INT(cmdInitMask("4294967280", &args), ArgsParseResultOk);
    TEST_ASSERT_EQUAL_UINT32(args.mask, 4294967280);

    InputArgs a2 = iaInit();
    TEST_ASSERT_EQUAL_INT(cmdInitMask("xyz", &a2), ArgsParseResultInvalidArg);
    TEST_ASSERT_EQUAL_INT(cmdInitMask("1.2.3.4/extra", &a2), ArgsParseResultInvalidArg);
}

// ---- cmdInitGateway ----

void testCmdInitGatewayIpOnly(void)
{
    InputArgs args = iaInit();

    TEST_ASSERT_EQUAL_INT(cmdInitGateway("192.168.1.1", &args), ArgsParseResultOk);
    TEST_ASSERT_TRUE(args.gatewayInputed);
    TEST_ASSERT_EQUAL_UINT32(args.gateway, 0xC0A80101u);
    TEST_ASSERT_FALSE(args.maskInputed); // маска не задана
}

void testCmdInitGatewayIpWithMask(void)
{
    InputArgs args = iaInit();

    TEST_ASSERT_EQUAL_INT(cmdInitGateway("192.168.1.1/24", &args), ArgsParseResultOk);
    TEST_ASSERT_TRUE(args.gatewayInputed);
    TEST_ASSERT_EQUAL_UINT32(args.gateway, 0xC0A80101u);
    TEST_ASSERT_TRUE(args.maskInputed);
    TEST_ASSERT_EQUAL_UINT32(args.mask, 24);
}

void testCmdInitGatewayInvalid(void)
{
    InputArgs a = iaInit();
    TEST_ASSERT_EQUAL_INT(cmdInitGateway("abc", &a), ArgsParseResultInvalidArg);
    TEST_ASSERT_EQUAL_INT(cmdInitGateway("1.2.3.4/xx", &a), ArgsParseResultInvalidArg);
}

// ---- cmdInitSeed ----

void testCmdInitSeed(void)
{
    InputArgs args = iaInit();

    TEST_ASSERT_EQUAL_INT(cmdInitSeed("42", &args), ArgsParseResultOk);
    TEST_ASSERT_TRUE(args.seedInputed);
    TEST_ASSERT_EQUAL_UINT32(args.seed, 42);

    TEST_ASSERT_EQUAL_INT(cmdInitSeed("43", &args), ArgsParseResultDoubleInit);
    args = iaInit();
    TEST_ASSERT_EQUAL_INT(cmdInitSeed("nope", &args), ArgsParseResultInvalidArg);
}

// ---- convertArgsParseResult ----

void testConvertArgsParseResult(void)
{
    TEST_ASSERT_NULL(convertArgsParseResult(ArgsParseResultOk));
    TEST_ASSERT_NOT_NULL(convertArgsParseResult(ArgsParseResultInvalidArg));
    TEST_ASSERT_NOT_NULL(convertArgsParseResult(ArgsParseResultFlagAtEnd));
    TEST_ASSERT_NOT_NULL(convertArgsParseResult(ArgsParseResultDoubleInit));
    TEST_ASSERT_NOT_NULL(convertArgsParseResult(ArgsParseResultDevError));
}

// ---- cmdFillCommands / cmdInitArgsWithFree ----

void testFullParseSuccess(void)
{
    char *argv[] = {
        "prog",
        "-g",
        "192.168.1.1/24",
        "-c",
        "5",
        "-s",
        "1",
    };
    int argc = 7;

    InputArgs args = iaInit();
    Commands  cmds = cmdFillCommands(
        4,
        "-c", "--count", cmdInitCount,
        "-m", "--mask", cmdInitMask,
        "-g", "--gateway", cmdInitGateway,
        "-s", "--seed", cmdInitSeed);

    ArgsParseResult res = cmdInitArgsWithFree(argc, argv, cmds, &args);

    TEST_ASSERT_EQUAL_INT(res, ArgsParseResultOk);

    TEST_ASSERT_TRUE(args.countInputed);
    TEST_ASSERT_EQUAL_UINT32(args.count, 5);
    TEST_ASSERT_TRUE(args.gatewayInputed);
    TEST_ASSERT_EQUAL_UINT32(args.gateway, 0xC0A80101u);
    TEST_ASSERT_TRUE(args.maskInputed);
    TEST_ASSERT_EQUAL_UINT32(args.mask, 24);
    TEST_ASSERT_TRUE(args.seedInputed);
    TEST_ASSERT_EQUAL_UINT32(args.seed, 1);
    TEST_ASSERT_TRUE(iaIsValid(&args));
}

void testFullParseLongFlags(void)
{
    char *argv[] = {"prog", "--count", "7"};
    int   argc = 3;

    InputArgs args = iaInit();
    Commands  cmds = cmdFillCommands(1, "-c", "--count", cmdInitCount);

    TEST_ASSERT_EQUAL_INT(cmdInitArgsWithFree(argc, argv, cmds, &args), ArgsParseResultOk);
    TEST_ASSERT_EQUAL_UINT32(args.count, 7);
}

void testFullParseFlagAtEnd(void)
{
    // Флаг без значения в конце
    char *argv[] = {"prog", "-c"};
    int   argc = 2;

    InputArgs args = iaInit();
    Commands  cmds = cmdFillCommands(1, "-c", "--count", cmdInitCount);

    TEST_ASSERT_EQUAL_INT(cmdInitArgsWithFree(argc, argv, cmds, &args), ArgsParseResultFlagAtEnd);
}

void testFullParseInvalidArg(void)
{
    char *argv[] = {"prog", "-c", "oops"};
    int   argc = 3;

    InputArgs args = iaInit();
    Commands  cmds = cmdFillCommands(1, "-c", "--count", cmdInitCount);

    TEST_ASSERT_EQUAL_INT(cmdInitArgsWithFree(argc, argv, cmds, &args), ArgsParseResultInvalidArg);
}

void testFullParseDoubleInit(void)
{
    char *argv[] = {"prog", "-c", "1", "-c", "2"};
    int   argc = 5;

    InputArgs args = iaInit();
    Commands  cmds = cmdFillCommands(1, "-c", "--count", cmdInitCount);

    TEST_ASSERT_EQUAL_INT(cmdInitArgsWithFree(argc, argv, cmds, &args), ArgsParseResultDoubleInit);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testCmdInitCount);
    RUN_TEST(testCmdInitMaskFromIp);
    RUN_TEST(testCmdInitMaskFromUInt32);
    RUN_TEST(testCmdInitGatewayIpOnly);
    RUN_TEST(testCmdInitGatewayIpWithMask);
    RUN_TEST(testCmdInitGatewayInvalid);
    RUN_TEST(testCmdInitSeed);
    RUN_TEST(testConvertArgsParseResult);
    RUN_TEST(testFullParseSuccess);
    RUN_TEST(testFullParseLongFlags);
    RUN_TEST(testFullParseFlagAtEnd);
    RUN_TEST(testFullParseInvalidArg);
    RUN_TEST(testFullParseDoubleInit);

    return UNITY_END();
}