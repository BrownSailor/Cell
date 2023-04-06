#include "compiler_utils.h"

static const uint64_t KEY_LSB = 62;
static const uint64_t TMP_LSB =  0;
static const uint64_t VAR_LSB = 20;
static const uint64_t LAB_LSB = 36;

static const uint64_t KEY_WID =  2;
static const uint64_t TMP_WID = 20;
static const uint64_t VAR_WID = 16;
static const uint64_t LAB_WID = 26;

static const uint64_t KEY_MASK = (((1ULL << KEY_WID) - 1) << KEY_LSB);
static const uint64_t TMP_MASK = (((1ULL << TMP_WID) - 1) << TMP_LSB);
static const uint64_t VAR_MASK = (((1ULL << VAR_WID) - 1) << VAR_LSB);
static const uint64_t LAB_MASK = (((1ULL << LAB_WID) - 1) << LAB_LSB);


uint64_t store_id(uint64_t word, Type type, uint64_t id)
{
    word &= ~KEY_MASK;
    word |= (uint64_t)type << KEY_LSB;

    switch (type)
    {
        /* temporary identifier */
        case TMP:
        {
            word &= ~TMP_MASK;
            return word | id;
        }
        case VAR:
        {
            word &= VAR_MASK;
            return word | (id << VAR_LSB);
        }
        case LAB:
        {
            word &= LAB_MASK;
            return word | (id << LAB_LSB);
        }
        default:
        {
            return 0;
        }
    }
}

uint64_t get_id(uint64_t word)
{
    uint64_t key = (word & KEY_MASK) >> KEY_LSB;

    switch (key)
    {
        /* temporary identifier */
        case 1:
        {
            return word & TMP_MASK;
        }
        case 2:
        {
            return (word & VAR_MASK) >> VAR_LSB;
        }
        case 3:
        {
            return (word & LAB_MASK) >> LAB_LSB;
        }
        default:
        {
            return 0;
        }
    }
}
