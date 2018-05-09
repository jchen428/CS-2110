#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include "my_malloc.h"
 
// IT'S BACK!!!!1!!!!1!
/*#define fail \
    do { \
            printf("%s ✗\n", __func__); \
            abort(); \
    } while (0)
 
#define pass \
    do { \
            printf("%s ✔\n", __func__); \
            return; \
    } while (0)
 
#define assert(cond) \
    do { \
            if (!(cond)) { \
                    fprintf(stderr, "Assertion failed: %s in %s at %s:%d.\n", \
                            #cond, __func__, __FILE__, __LINE__); \
                    fail; \
            } \
    } while (0)
 
extern void *heap;
extern metadata_t *freelist[8];
 
static void test_that_allocation_sets_heap()
{
    void *block = my_malloc(2048 - sizeof(metadata_t));
    assert(block != NULL);
    uintptr_t h = (uintptr_t) heap;
    uintptr_t b = (uintptr_t) block;
    assert(h == (b - sizeof(metadata_t)));
    my_free(block);
    pass;
}
 
static void test_allocating_bad_size()
{
    void *block = my_malloc(0);
    assert(block == NULL);
    block = my_malloc(2049 - sizeof(metadata_t));
    assert(block == NULL);
    assert(ERRNO == SINGLE_REQUEST_TOO_LARGE);
    pass;
}
 
static int compare_uintptr(const void *p1, const void *p2)
{
    return *(const uintptr_t *)p1 - *(const uintptr_t *)p2;
}
 
static void test_allocation_of_blocks(size_t size)
{
    int num_blocks = (8192 / size) + 1;
    int blocksize = 1 << (int) ceil(log2(size));
    uintptr_t blocks[num_blocks];
    uintptr_t h = (uintptr_t) heap;
    for (int i = 0; i < num_blocks; i++)
        blocks[i] = (uintptr_t) my_malloc(size - sizeof(metadata_t));
 
    metadata_t *metas[num_blocks - 1];
    for (int i = 0; i < num_blocks - 1; i++)
        metas[i] = (metadata_t *) blocks[i] - 1;
 
    qsort(blocks, num_blocks - 1, sizeof(uintptr_t), compare_uintptr);
 
    assert(blocks[0] == h + sizeof(metadata_t));
    assert(metas[0]->in_use);
    assert(metas[0]->size == blocksize);
    assert(metas[0]->next == NULL);
    assert(metas[0]->prev == NULL);
 
    for (int i = 1; i < num_blocks - 1; i++) {
        assert(blocks[i] == blocks[i - 1] + size);
        assert(metas[i]->in_use);
        assert(metas[i]->size == blocksize);
        assert(metas[i]->next == NULL);
        assert(metas[i]->prev == NULL);
    }
 
    assert(blocks[num_blocks - 1] == 0); // no more memory left, should be NULL
 
    for (int i = 0; i < num_blocks - 1; i++)
        my_free((void *) blocks[i]);
}
 
static void test_allocation_of_2048_blocks()
{
    test_allocation_of_blocks(2048);
    for (int i = 0; i < 7; i++)
        assert(freelist[i] == NULL);
    pass;
}
 
static void test_allocation_of_1024_blocks()
{
    test_allocation_of_blocks(1024);
    for (int i = 0; i < 7; i++)
        assert(freelist[i] == NULL);
    pass;
}
 
static void test_allocation_of_512_blocks()
{
    test_allocation_of_blocks(512);
    for (int i = 0; i < 7; i++)
        assert(freelist[i] == NULL);
    pass;
}
 
static void test_allocation_of_256_blocks()
{
    test_allocation_of_blocks(256);
    for (int i = 0; i < 7; i++)
        assert(freelist[i] == NULL);
    pass;
}
 
static void test_allocation_of_128_blocks()
{
    test_allocation_of_blocks(128);
    for (int i = 0; i < 7; i++)
        assert(freelist[i] == NULL);
    pass;
}
 
static void test_allocation_of_64_blocks()
{
    test_allocation_of_blocks(64);
    for (int i = 0; i < 7; i++)
        assert(freelist[i] == NULL);
    pass;
}
 
static void test_allocation_of_32_blocks()
{
    test_allocation_of_blocks(32);
    for (int i = 0; i < 7; i++)
        assert(freelist[i] == NULL);
    pass;
}
 
static void test_allocation_of_16_blocks()
{
    if (sizeof(metadata_t) < 16) {
        test_allocation_of_blocks(16);
        for (int i = 0; i < 7; i++)
            assert(freelist[i] == NULL);
    }
    pass;
}
 
static void test_intensive_random_allocations()
{
    // allocate all 8192 bytes and free it so the freelists are accurate
    void *b1 = my_malloc(2048 - sizeof(metadata_t));
    void *b2 = my_malloc(2048 - sizeof(metadata_t));
    void *b3 = my_malloc(2048 - sizeof(metadata_t));
    void *b4 = my_malloc(2048 - sizeof(metadata_t));
    my_free(b1);
    my_free(b2);
    my_free(b3);
    my_free(b4);
 
    int allocatedsize = 0;
    int allocated = 0;
    void *blocks[500];
 
    for (int i = 0; i < 1000000; i++) {
        int op;
        if (allocated == 0) {
            op = 0;
        } else if (allocatedsize >= (8192 - sizeof(metadata_t))) {
            op = 1;
        } else {
            op = rand() % 2;
        }
        switch (op) {
            case 0: {
                int size;
                if (8192 - allocatedsize < 2048) {
                    int maxsize = 1 << (int) floor(log2(8192 - allocatedsize));
                    size = (rand() % (maxsize - sizeof(metadata_t))) + 1;
                } else {
                    size = (rand() % (2048 - sizeof(metadata_t))) + 1;
                }
 
                int blocksize = 1 << (int) ceil(log2(size + sizeof(metadata_t)));
                allocatedsize += blocksize;
                blocks[allocated++] = my_malloc(size);
                assert((((metadata_t *)blocks[allocated - 1]) - 1)->size == blocksize);
                break;
            }
            case 1: {
                void *block = blocks[--allocated];
                metadata_t *meta = ((metadata_t *) block) - 1;
                allocatedsize -= meta->size;
                my_free(block);
                assert(!meta->in_use);
 
                break;
            }
            default:
                // shouldn't ever happen
                break;
        }
 
        void *blocks_sorted[500];
        for (int i = 0; i < 500; i++) {
            blocks_sorted[i] = blocks[i];
        }
 
        qsort(blocks_sorted, allocated, sizeof(void *), compare_uintptr);
 
        for (int i = 0; i < allocated - 1; i++) {
            metadata_t *meta = ((metadata_t *) blocks_sorted[i]) - 1;
            int size = meta->size;
            uintptr_t addr1 = (uintptr_t) meta;
            metadata_t *meta2 = ((metadata_t *) blocks_sorted[i + 1]) - 1;
            uintptr_t addr2 = (uintptr_t) meta2;
 
            assert(meta->in_use); // verify both blocks in use
            assert(meta2->in_use);
 
            assert(addr2 >= addr1 + size); // verify blocks do not overlap
        }
 
        int remaining = 0;
        for (int i = 0; i < 8; i++) {
            metadata_t *m = freelist[i];
            while (m) {
                remaining += m->size;
                m = m->next;
            }
        }
 
        assert(remaining == 8192 - allocatedsize);
    }
 
    for (int i = 0; i < allocated; i++)
        my_free(blocks[i]);
 
    for (int i = 0; i < 7; i++)
        assert(freelist[i] == NULL);
 
    pass;
}
 
static void test_calloc()
{
    int *p = my_calloc(250, sizeof(int));
 
    for (int i = 0; i < 250; i++)
        assert(p[i] == 0);
 
    my_free(p);
    pass;
}
 
static void test_memmove()
{
    int *space = my_malloc(2048 - sizeof(metadata_t));
    assert(space != NULL);
    for (int i = 0; i < 100; i++)
        space[i] = i;
    my_memmove(space + 50, space, 100 * sizeof(int));
    for (int i = 0; i < 100; i++)
        assert(space[i + 50] == i);
    my_memmove(space, space + 50, 100 * sizeof(int));
    for (int i = 0; i < 100; i++)
        assert(space[i] == i);
    my_free(space);
    pass;
}
 
static void test_setting_error_conditions()
{
    assert(!my_malloc(2048 - sizeof(metadata_t) + 1));
    assert(ERRNO == SINGLE_REQUEST_TOO_LARGE);
    void *blocks[4];
    for (int i = 0; i < 4; i++) {
        blocks[i] = my_malloc(2048 - sizeof(metadata_t));
        assert(blocks[i]);
        assert(ERRNO == NO_ERROR);
    }
    assert(!my_malloc(1));
    assert(ERRNO == OUT_OF_MEMORY);
    for (int i = 0; i < 4; i++) {
        my_free(blocks[i]);
        assert(ERRNO == NO_ERROR);
    }
 
    pass;
}*/
 
int main() {
    /*int seed = time(NULL);
    srand(seed);
 
    test_that_allocation_sets_heap();
    test_allocating_bad_size();
    test_allocation_of_2048_blocks();
    test_allocation_of_1024_blocks();
    test_allocation_of_512_blocks();
    test_allocation_of_256_blocks();
    test_allocation_of_128_blocks();
    test_allocation_of_64_blocks();
    test_allocation_of_32_blocks();
    test_allocation_of_16_blocks();
    test_intensive_random_allocations();
    test_calloc();
    test_memmove();
    test_setting_error_conditions();
 
    free(heap); // free the fake-sbrk-but-actually-malloc'd heap*/
 
    return 0;
}