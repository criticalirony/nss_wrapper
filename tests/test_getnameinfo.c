#include "config.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

static void test_nwrap_getnameinfo(void **state)
{
	char host[256] = {0};
	char serv[256] = {0};
	struct sockaddr_in sin;
	struct sockaddr_in6 sin6;
	int flags = 0;
	int rc;

	(void) state; /* unused */

	/* IPv4 */
	sin.sin_family = AF_INET;
	sin.sin_port = htons(53);
	rc = inet_pton(AF_INET, "127.0.0.11", &sin.sin_addr);
	assert_int_equal(rc, 1);

	rc = getnameinfo((const struct sockaddr *)&sin,
			 sizeof(struct sockaddr_in),
			 host, sizeof(host),
			 serv, sizeof(serv),
			 flags);
	assert_int_equal(rc, 0);

	assert_string_equal(host, "magrathea.galaxy.site");
	assert_string_equal(serv, "domain");

	/* IPv6 */
	sin6.sin6_family = AF_INET6;
	sin6.sin6_port = htons(53);
	rc = inet_pton(AF_INET6, "::13", &sin6.sin6_addr);
	assert_int_equal(rc, 1);

	rc = getnameinfo((const struct sockaddr *)&sin6,
			 sizeof(struct sockaddr_in6),
			 host, sizeof(host),
			 serv, sizeof(serv),
			 flags);
	assert_int_equal(rc, 0);

	assert_string_equal(host, "beteigeuze.galaxy.site");
	assert_string_equal(serv, "domain");
}

static void test_nwrap_getnameinfo_numeric(void **state)
{
	char host[256] = {0};
	char serv[256] = {0};
	struct sockaddr_in sin;
	struct sockaddr_in6 sin6;
	int flags = 0;
	int rc;

	(void) state; /* unused */

	/* IPv4 */
	sin.sin_family = AF_INET;
	sin.sin_port = htons(53);
	rc = inet_pton(AF_INET, "127.0.0.11", &sin.sin_addr);
	assert_int_equal(rc, 1);

	flags = NI_NUMERICHOST;

	rc = getnameinfo((const struct sockaddr *)&sin,
			 sizeof(struct sockaddr_in),
			 host, sizeof(host),
			 serv, sizeof(serv),
			 flags);
	assert_int_equal(rc, 0);

	assert_string_equal(host, "127.0.0.11");
	assert_string_equal(serv, "domain");

	/* IPv6 */
	sin6.sin6_family = AF_INET6;
	sin6.sin6_port = htons(53);
	rc = inet_pton(AF_INET6, "::13", &sin6.sin6_addr);
	assert_int_equal(rc, 1);

	flags = NI_NUMERICSERV;

	rc = getnameinfo((const struct sockaddr *)&sin6,
			 sizeof(struct sockaddr_in6),
			 host, sizeof(host),
			 serv, sizeof(serv),
			 flags);
	assert_int_equal(rc, 0);

	assert_string_equal(host, "beteigeuze.galaxy.site");
	assert_string_equal(serv, "53");
}

static void test_nwrap_getnameinfo_any(void **state)
{
	char host[256] = {0};
	char serv[256] = {0};
	struct sockaddr_in sin;
	struct sockaddr_in6 sin6;
	int flags = 0;
	int rc;

	(void) state; /* unused */

	/* IPv4 */
	sin.sin_family = AF_INET;
	sin.sin_port = htons(22);
	rc = inet_pton(AF_INET, "0.0.0.0", &sin.sin_addr);
	assert_int_equal(rc, 1);

	rc = getnameinfo((const struct sockaddr *)&sin,
			 sizeof(struct sockaddr_in),
			 host, sizeof(host),
			 serv, sizeof(serv),
			 flags);
	assert_int_equal(rc, 0);

	assert_string_equal(host, "0.0.0.0");
	assert_string_equal(serv, "ssh");

	/* IPv6 */
	sin6.sin6_family = AF_INET6;
	sin6.sin6_port = htons(22);
	rc = inet_pton(AF_INET6, "::", &sin6.sin6_addr);
	assert_int_equal(rc, 1);

	rc = getnameinfo((const struct sockaddr *)&sin6,
			 sizeof(struct sockaddr_in6),
			 host, sizeof(host),
			 serv, sizeof(serv),
			 flags);
	assert_int_equal(rc, 0);

	assert_string_equal(host, "::");
	assert_string_equal(serv, "ssh");
}

static void test_nwrap_getnameinfo_local(void **state)
{
	char host[256] = {0};
	char serv[256] = {0};
	struct sockaddr_in sin;
	struct sockaddr_in6 sin6;
	int flags = 0;
	int rc;

	(void) state; /* unused */

	/* IPv4 */
	sin.sin_family = AF_INET;
	sin.sin_port = htons(22);
	rc = inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);
	assert_int_equal(rc, 1);

	rc = getnameinfo((const struct sockaddr *)&sin,
			 sizeof(struct sockaddr_in),
			 host, sizeof(host),
			 serv, sizeof(serv),
			 flags);
	assert_int_equal(rc, 0);

	assert_string_equal(host, "127.0.0.1");
	assert_string_equal(serv, "ssh");

	/* IPv6 */
	sin6.sin6_family = AF_INET6;
	sin6.sin6_port = htons(22);
	rc = inet_pton(AF_INET6, "::1", &sin6.sin6_addr);
	assert_int_equal(rc, 1);

	rc = getnameinfo((const struct sockaddr *)&sin6,
			 sizeof(struct sockaddr_in6),
			 host, sizeof(host),
			 serv, sizeof(serv),
			 flags);
	assert_int_equal(rc, 0);

	assert_string_equal(host, "::1");
	assert_string_equal(serv, "ssh");
}

static void test_nwrap_getnameinfo_null(void **state)
{
	char host[256] = {0};
	char serv[256] = {0};
	struct sockaddr_in sin;
	struct sockaddr_in6 sin6;
	int flags = 0;
	int rc;

	(void) state; /* unused */

	rc = getnameinfo(NULL,
			 0,
			 host, sizeof(host),
			 serv, sizeof(serv),
			 flags);
	assert_int_equal(rc, EAI_FAMILY);

	/* IPv4 */
	sin.sin_family = AF_INET;
	sin.sin_port = htons(22);
	rc = inet_pton(AF_INET, "127.0.0.11", &sin.sin_addr);
	assert_int_equal(rc, 1);

	rc = getnameinfo((const struct sockaddr *)&sin,
			 sizeof(struct sockaddr_in),
			 NULL, 0,
			 serv, sizeof(serv),
			 flags);
	assert_int_equal(rc, 0);

	assert_string_equal(serv, "ssh");

	/* IPv6 */
	sin6.sin6_family = AF_INET6;
	sin6.sin6_port = htons(22);
	rc = inet_pton(AF_INET6, "::13", &sin6.sin6_addr);
	assert_int_equal(rc, 1);

	rc = getnameinfo((const struct sockaddr *)&sin6,
			 sizeof(struct sockaddr_in6),
			 host, sizeof(host),
			 NULL, 0,
			 flags);
	assert_int_equal(rc, 0);

	assert_string_equal(host, "beteigeuze.galaxy.site");
}

int main(void) {
	int rc;

	const UnitTest tests[] = {
		unit_test(test_nwrap_getnameinfo),
		unit_test(test_nwrap_getnameinfo_numeric),
		unit_test(test_nwrap_getnameinfo_any),
		unit_test(test_nwrap_getnameinfo_local),
		unit_test(test_nwrap_getnameinfo_null),
	};

	rc = run_tests(tests);

	return rc;
}
