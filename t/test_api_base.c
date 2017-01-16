/**
 *  @brief Base API Test Suite.
 *
 *  @details
 *  Test the functionality of the #offsetof() macro provided by the Base API.
 *
 *  Additionally, test the byte swapping functions #htobe64 and #htole64.
 */

#define _BSD_SOURCE
#include <endian.h>             /* htobe64, htole64 */
#include <stdint.h>             /* typedef */
#include <limits.h>             /* *_MIN, *_MAX */

#include <tap.h>                /* libtap */

#if defined __GNUC__
#define __packed __attribute((packed))
#endif

/* Magic quote macro */
#define QUOTE_(s) #s
#define QUOTE(s) QUOTE_(s)

/* short typedefs */
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

/**
 *  @brief Evaluates to the offset (in bytes) of M within T.
 *
 *  @details For more information, see:
 *
 *  - <https://en.wikipedia.org/wiki/offsetof>
 *  - <http://en.cppreference.com/w/cpp/types/offsetof>
 *  - <https://msdn.microsoft.com/en-us/library/dz4y9b9a.aspx>
 *  - <http://www.cplusplus.com/reference/cstddef/offsetof/>
 *
 * @param t The structure or union type to used evaluate T->M.
 * @param m The member of T to evaluate the offset (in bytes) of.
 * @return size_t representing the offset in bytes of T->M.
 */
#define offsetof(t,m) ((size_t)&(((t*)0)->m))

typedef struct __packed _Au
{
  u64 a;
  u32 b;
  u16 c;
  u8 d;
} Au;

typedef struct __packed _Ai
{
  i64 a;
  i32 b;
  i16 c;
  i8 d;
} Ai;

typedef struct __packed _B
{
  union
  {
    u8 a[8];
    u64 b;
  };
} B;

int
main (void)
{
  Ai a1;
  Au a2;
  B b;
  u64 val[3];

  /* Tedious to keep track of until testing is finalized. */
  plan (NO_PLAN);

  /* Size checks. */
  ok (sizeof (i64) + sizeof (i32) + sizeof (i16) + sizeof (i8) == sizeof (Ai), QUOTE (__packed));
  ok (sizeof (u64) + sizeof (u32) + sizeof (u16) + sizeof (u8) == sizeof (Au), QUOTE (__packed));

  ok (sizeof (i64) + sizeof (i32) + sizeof (i16) + sizeof (i8) == sizeof a1, QUOTE (__packed));
  ok (sizeof (u64) + sizeof (u32) + sizeof (u16) + sizeof (u8) == sizeof a2, QUOTE (__packed));

  ok (sizeof (u64) == sizeof (B), QUOTE (__packed));
  ok (sizeof (u64) == sizeof b, QUOTE (__packed));

  /* Test packed structure semantics. */
  val[0] = sizeof a1.a; val[1] = sizeof a1.b; val[2] = sizeof a1.c;
  cmp_ok (val[0] + val[1] + val[2], "==", offsetof (Ai, d), QUOTE (__packed));

  val[0] = sizeof a2.a; val[1] = sizeof a2.b; val[2] = sizeof a2.c;
  cmp_ok (val[0] + val[1] + val[2], "==", offsetof (Au, d), QUOTE (__packed));

  /* Test structure { union {} } semantics. */
  cmp_ok (0, "==", offsetof (B, a), "0 == &(struct{union{a,b}})->a");
  cmp_ok (0, "==", offsetof (B, b), "0 == &(struct{union{a,b}})->b");

  /* Unsigned underflow test. */
  a2.a  =    a2.b =     a2.c =     a2.d = 0;
  a2.a -= 1; a2.b -= 1; a2.c -= 1; a2.d -= 1;
  ok ((ULONG_MAX == a2.a),             "unsigned underflow - 64 bits");
  cmp_ok (UINT_MAX       , "==", a2.b, "unsigned underflow - 32 bits");
  cmp_ok (USHRT_MAX      , "==", a2.c, "unsigned underflow - 16 bits");
  cmp_ok (UCHAR_MAX      , "==", a2.d, "unsigned underflow - 8 bits");

  /* Unsigned overflow test. */
  a2.a = ULONG_MAX; a2.b = UINT_MAX; a2.c = USHRT_MAX; a2.d = UCHAR_MAX;
  a2.a += 1       ; a2.b += 1      ; a2.c += 1       ; a2.d += 1       ;
  cmp_ok (0, "==", a2.a, "unsigned overflow - 64 bits");
  cmp_ok (0, "==", a2.b, "unsigned overflow - 32 bits");
  cmp_ok (0, "==", a2.c, "unsigned overflow - 16 bits");
  cmp_ok (0, "==", a2.d, "unsigned overflow - 8 bits");

  /* Signed underflow test. */
  a1.a = LONG_MIN; a1.b = INT_MIN; a1.c = SHRT_MIN; a1.d = CHAR_MIN;
  a1.a -= 1      ; a1.b -= 1     ; a1.c -= 1      ; a1.d -= 1      ;
  ok ((LONG_MAX == a1.a),       "signed underflow - 64 bits");
  cmp_ok (INT_MAX,  "==", a1.b, "signed underflow - 32 bits");
  cmp_ok (SHRT_MAX, "==", a1.c, "signed underflow - 16 bits");
  cmp_ok (CHAR_MAX, "==", a1.d, "signed underflow - 8 bits");

  /* Signed overflow test. */
  a1.a = LONG_MAX; a1.b = INT_MAX; a1.c = SHRT_MAX; a1.d = CHAR_MAX;
  a1.a += 1      ; a1.b += 1     ; a1.c += 1      ; a1.d += 1      ;
  ok ((LONG_MIN == a1.a),  	"signed overflow - 64 bits");
  cmp_ok (INT_MIN,  "==", a1.b, "signed overflow - 32 bits");
  cmp_ok (SHRT_MIN, "==", a1.c, "signed overflow - 16 bits");
  cmp_ok (CHAR_MIN, "==", a1.d, "signed overflow - 8 bits");

  /* Unsigned integer memory occupation tests. */
  const void *ptr1 = b.a,
             *ptr2 = (const void *) &val[1];
  val[0] = 0xfedcba0123456789;
  val[1] = htole64 (val[0]);
  b.b    = htole64 (val[0]);
  cmp_mem (ptr1, ptr2, 8, "0x%lx == 0x%lx", *(u64*)ptr1, *(u64*)ptr2);

  val[1] = htobe64 (val[0]);
  b.b    = htobe64 (val[0]);
  cmp_mem (ptr1, ptr2, 8, "0x%lx == 0x%lx", *(u64*)ptr1, *(u64*)ptr2);

  /* display summary */
  done_testing ();

  return 0;
}
