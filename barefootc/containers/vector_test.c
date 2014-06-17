
#define BFC_VECTOR_ENABLE_TRIPLE_INDIRECT		1
#include "barefootc/vector.h"

typedef struct test {
	int	a;
	char	b[2];
} test_t;

typedef BFC_VECTOR (testvec, test_t, 4) testvec_t;

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int
main (int argc, char *argv[])
{
	int *ip, ib[20];
	test_t *tp, tb[20];
	testvec_t tvec;
	bfc_int_vector_t ivec;
	int i, total_iterations = 0;

	BFC_VECTOR_INIT(&ivec);
	BFC_VECTOR_INIT(&tvec);

	BFC_VECTOR_SET(&ivec, 27, 127);
	BFC_VECTOR_SET(&ivec, 10000, 11222);
	BFC_VECTOR_SET(&ivec, 10000000, 11222333);
	BFC_VECTOR_PUSH_BACK(&ivec, 11222334);
	ip   =   BFC_VECTOR_REF (&ivec, 27);
	printf ("BFC_VECTOR_REF (&ivec, 27) = %p, %d\n", ip, ip? *ip: 0);
	ip   =   BFC_VECTOR_REF (&ivec, 10000);
	printf ("BFC_VECTOR_REF (&ivec, 10000) = %p, %d\n", ip, ip? *ip: 0);
	ip   =   BFC_VECTOR_REF (&ivec, 10000000);
	printf ("BFC_VECTOR_REF (&ivec, 10000000) = %p, %d\n", ip, ip? *ip: 0);
	ip   =   BFC_VECTOR_REF (&ivec, 10000001);
	printf ("BFC_VECTOR_REF (&ivec, 10000001) = %p, %d\n", ip, ip? *ip: 0);
	BFC_VECTOR_INSERT(&ivec, 10000000, 11222332);
	printf ("BFC_VECTOR_INSERT (&ivec, 10000000, 11222332)\n");
	ip   =   BFC_VECTOR_REF (&ivec, 10000000);
	printf ("BFC_VECTOR_REF (&ivec, 10000000) = %p, %d\n", ip, ip? *ip: 0);
	ip   =   BFC_VECTOR_REF (&ivec, 10000001);
	printf ("BFC_VECTOR_REF (&ivec, 10000001) = %p, %d\n", ip, ip? *ip: 0);
	ip   =   BFC_VECTOR_REF (&ivec, 10000002);
	printf ("BFC_VECTOR_REF (&ivec, 10000002) = %p, %d\n", ip, ip? *ip: 0);
	BFC_VECTOR_DELETE(&ivec, 10000001);
	printf ("BFC_VECTOR_DELETE (&ivec, 10000001)\n");
	ip   =   BFC_VECTOR_REF (&ivec, 10000000);
	printf ("BFC_VECTOR_REF (&ivec, 10000000) = %p, %d\n", ip, ip? *ip: 0);
	ip   =   BFC_VECTOR_REF (&ivec, 10000001);
	printf ("BFC_VECTOR_REF (&ivec, 10000001) = %p, %d\n", ip, ip? *ip: 0);
	ip   =   bfc_vector_ref (&ivec, 27);
	printf ("bfc_vector_ref (&ivec, 27) = %p, %d\n", ip, ip? *ip: 0);
	ip   =   bfc_vector_ref (&ivec, 10000000);
	printf ("bfc_vector_ref (&ivec, 10000000) = %p, %d\n", ip, ip? *ip: 0);
	ip   =   bfc_vector_ref (&ivec, 10000001);
	printf ("bfc_vector_ref (&ivec, 10000001) = %p, %d\n", ip, ip? *ip: 0);

	printf ("ivec size %u\n", BFC_VECTOR_GET_SIZE(&ivec));
	printf ("ivec direct %u @%p\n", CV0_ELEMENTS(&ivec), ivec.direct);
	printf ("ivec indirect %u @%p\n", CV1_ELEMENTS(&ivec), ivec.indirect);
	printf ("ivec double indirect %u @%p\n",
				CV2_ELEMENTS(&ivec), ivec.double_indirect);
	printf ("ivec triple indirect %u @%p\n",
				CV3_ELEMENTS(&ivec), ivec.triple_indirect);

	total_iterations = 0;
	BFC_VECTOR_FOREACH(ip, i, &ivec) {
		total_iterations++;
		switch (i) {
		case 27:
			printf ("ivec[%d]: %d @%p\n", i, *ip, ip);
			if (*ip != 127) {
				printf ("ivec[%d]: %d @%p, expected %d\n",
					i, *ip, ip, 127);
			}
			break;
		case 10000:
			printf ("ivec[%d]: %d @%p\n", i, *ip, ip);
			if (*ip != 11222) {
				printf ("ivec[%d]: %d @%p, expected %d\n",
					i, *ip, ip, 11222);
			}
			break;
		case 10000000:
			printf ("ivec[%d]: %d @%p\n", i, *ip, ip);
			if (*ip != 11222332) {
				printf ("ivec[%d]: %d @%p, expected %d\n",
					i, *ip, ip, 11222332);
			}
			break;
		case 10000001:
			printf ("ivec[%d]: %d @%p\n", i, *ip, ip);
			if (*ip != 11222334) {
				printf ("ivec[%d]: %d @%p, expected %d\n",
					i, *ip, ip, 11222334);
			}
			break;
		case 10000002:
			printf ("ivec[%d]: %d @%p\n", i, *ip, ip);
			/* undefined after delete */
			break;
		default:
			if (*ip != 0) {
				printf ("ivec[%d]: %d @%p, expected %d\n",
					i, *ip, ip, 0);
			}
		}
	} END_BFC_VECTOR_FOREACH;
	printf ("BFC_VECTOR_FOREACH(ivec) iterated %d times\n", total_iterations);

	tb[0].a = 0;
	tb[0].b[0] = 1;
	tb[0].b[1] = 2;
	BFC_VECTOR_SET (&tvec, 0, tb[0]);
	tb[0].a = 17;
	tb[0].b[0] = 1;
	tb[0].b[1] = 2;
	BFC_VECTOR_SET (&tvec, 17, tb[0]);
	tb[17] = BFC_VECTOR_GET (&tvec, 17);
	printf ("BFC_VECTOR_GET (&tvec, 17) = %d, %02x %02x\n",
		tb[17].a, tb[17].b[0], tb[17].b[1]);
	tb[18] = BFC_VECTOR_GET (&tvec, 18);
	printf ("BFC_VECTOR_GET (&tvec, 18) = %d, %02x %02x\n",
		tb[18].a, tb[18].b[0], tb[18].b[1]);
	tb[10] = BFC_VECTOR_GET (&tvec, 10018);
	printf ("BFC_VECTOR_GET (&tvec, 10018) = %d, %02x %02x\n",
		tb[10].a, tb[10].b[0], tb[10].b[1]);

	printf ("tvec size %u\n", BFC_VECTOR_GET_SIZE(&tvec));
	printf ("tvec direct %u @%p\n", CV0_ELEMENTS(&tvec), tvec.direct);
	printf ("tvec indirect %u @%p\n", CV1_ELEMENTS(&tvec), tvec.indirect);
	printf ("tvec double indirect %u @%p\n",
				CV2_ELEMENTS(&tvec), tvec.double_indirect);
	printf ("tvec triple indirect %u @%p\n",
				CV3_ELEMENTS(&tvec), tvec.triple_indirect);

	BFC_VECTOR_DESTROY(&ivec);
	BFC_VECTOR_DESTROY(&tvec);

	exit (0);
}


