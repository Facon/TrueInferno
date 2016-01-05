// basics.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "vld.h"
#include "Allocs.h"

#include <gtest/gtest.h>

class TestBasics : public ::testing::TestWithParam<bool>
{
    virtual void SetUp()
    {
        VLDMarkAllLeaksAsReported();
    }
    virtual void TearDown()
    {
        // Check that callstack resolved without unresolved functions (required symbols for all dll's)
        //EXPECT_EQ(0, VLDResolveCallstacks());
    }
};

TEST_P(TestBasics, Malloc)
{
    int prev = static_cast<int>(VLDGetLeaksCount());
    LeakMemory(eMalloc, repeats, GetParam());
    int total = static_cast<int>(VLDGetLeaksCount());
    int leaks = total - prev;
    int correctLeaks = GetParam() ? 0 : repeats * 2;
    ASSERT_EQ(correctLeaks, leaks);
}

TEST_P(TestBasics, New)
{
    int prev = static_cast<int>(VLDGetLeaksCount());
    LeakMemory(eNew, repeats, GetParam());
    int total = static_cast<int>(VLDGetLeaksCount());
    int leaks = total - prev;
    int correctLeaks = GetParam() ? 0 : repeats * 2;
    ASSERT_EQ(correctLeaks, leaks);
}

TEST_P(TestBasics, NewArray)
{
    int prev = static_cast<int>(VLDGetLeaksCount());
    LeakMemory(eNewArray, repeats, GetParam());
    int total = static_cast<int>(VLDGetLeaksCount());
    int leaks = total - prev;
    int correctLeaks = GetParam() ? 0 : repeats * 2;
    ASSERT_EQ(correctLeaks, leaks);
}

TEST_P(TestBasics, Calloc)
{
    int prev = static_cast<int>(VLDGetLeaksCount());
    LeakMemory(eCalloc, repeats, GetParam());
    int total = static_cast<int>(VLDGetLeaksCount());
    int leaks = total - prev;
    int correctLeaks = GetParam() ? 0 : repeats * 2;
    ASSERT_EQ(correctLeaks, leaks);
}

TEST_P(TestBasics, Realloc)
{
    int prev = static_cast<int>(VLDGetLeaksCount());
    LeakMemory(eRealloc, repeats, GetParam());
    int total = static_cast<int>(VLDGetLeaksCount());
    int leaks = total - prev;
    int correctLeaks = GetParam() ? 0 : repeats * 2;
    ASSERT_EQ(correctLeaks, leaks);
}

TEST_P(TestBasics, CoTaskMem)
{
    int prev = static_cast<int>(VLDGetLeaksCount());
    LeakMemory(eCoTaskMem, repeats, GetParam());
    int total = static_cast<int>(VLDGetLeaksCount());
    int leaks = total - prev;
    int correctLeaks = GetParam() ? 0 : repeats * 2;
    ASSERT_EQ(correctLeaks, leaks);
}

TEST_P(TestBasics, AlignedMalloc)
{
    int prev = static_cast<int>(VLDGetLeaksCount());
    LeakMemory(eAlignedMalloc, repeats, GetParam());
    int total = static_cast<int>(VLDGetLeaksCount());
    int leaks = total - prev;
    int correctLeaks = GetParam() ? 0 : repeats * 3;
    ASSERT_EQ(correctLeaks, leaks);
}

TEST_P(TestBasics, AlignedRealloc)
{
    int prev = static_cast<int>(VLDGetLeaksCount());
    LeakMemory(eAlignedRealloc, repeats, GetParam());
    int total = static_cast<int>(VLDGetLeaksCount());
    int leaks = total - prev;
    int correctLeaks = GetParam() ? 0 : repeats * 3;
    ASSERT_EQ(correctLeaks, leaks);
}

TEST_P(TestBasics, Strdup)
{
    int prev = static_cast<int>(VLDGetLeaksCount());
    LeakMemory(eStrdup, repeats, GetParam());
    int total = static_cast<int>(VLDGetLeaksCount());
    int leaks = total - prev;
    int correctLeaks = GetParam() ? 0 : repeats * 4;
    ASSERT_EQ(correctLeaks, leaks);
}

TEST_P(TestBasics, HeapAlloc)
{
    int prev = static_cast<int>(VLDGetLeaksCount());
    LeakMemory(eHeapAlloc, repeats, GetParam());
    int total = static_cast<int>(VLDGetLeaksCount());
    int leaks = total - prev;
    int correctLeaks = GetParam() ? 0 : repeats * 1;
    ASSERT_EQ(correctLeaks, leaks);
}

TEST_P(TestBasics, IMalloc)
{
    int prev = static_cast<int>(VLDGetLeaksCount());
    LeakMemory(eIMalloc, repeats, GetParam());
    int total = static_cast<int>(VLDGetLeaksCount());
    int leaks = total - prev;
    int correctLeaks = GetParam() ? 0 : repeats * 1;
    ASSERT_EQ(correctLeaks, leaks);
}

TEST_P(TestBasics, GetProcMalloc)
{
    int prev = static_cast<int>(VLDGetLeaksCount());
    LeakMemory(eGetProcMalloc, repeats, GetParam());
    int total = static_cast<int>(VLDGetLeaksCount());
    int leaks = total - prev;
    int correctLeaks = GetParam() ? 0 : repeats * 1;
    ASSERT_EQ(correctLeaks, leaks);
}

INSTANTIATE_TEST_CASE_P(FreeVal,
    TestBasics,
    ::testing::Bool());

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int res = RUN_ALL_TESTS();
    VLDMarkAllLeaksAsReported();
    return res;
}
