/* bootstrap project builder
 * compile with:
 *  cc build.c -o build
 *
 * quite possibly the 2nd worst code ive written
 */
#define CC     "clang"
#define CFLAGS "-Wall", "-O3", "-std=c23"
#define LDFLAGS "-lm"

#define RENDER_DEPENDENCIES() \
        DEPENDS_ON("vec")     \
        DEPENDS_ON("ppm")     \
        DEPENDS_ON("model")   \
        DEPENDS_ON("bvh")     \
        DEPENDS_ON("pfor")    \
        DEPENDS_ON("camera")  \
        DEPENDS_ON("render")  \
        DEPENDS_ON("random")  \
        DEPENDS_ON("object")  \
        DEPENDS_ON("scene")

#define TEST_DEPENDENCIES()       \
        DEPENDS_ON("test_vec")    \
        DEPENDS_ON("test_ppm")    \
        DEPENDS_ON("test_model")  \
        DEPENDS_ON("test_bvh")    \
        DEPENDS_ON("test_camera") \
        DEPENDS_ON("test_render") \
        DEPENDS_ON("test_random") \
        DEPENDS_ON("test_object")

/* build logic */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
	#include <windows.h>
	#define STAT_T struct _stat
	#define STAT   _stat
#else
	#include <sys/stat.h>
	#define STAT_T struct stat
	#define STAT   stat
#endif

#define CMD_LENGTH 1024

int
is_newer(
	const char *file_a,
	const char *file_b
)
{
	/* return true if file_a is newer than file_b */
	STAT_T a;
	STAT_T b;

	if (STAT(file_a, &a))
		return 0;
	if (STAT(file_b, &b))
		return 1;
	return a.st_mtime > b.st_mtime;
}

int
compile_if_newer(
	const char *object,
	const char *source,
	const char **dependencies
)
{
	int compile = is_newer(source, object);
	for (size_t i = 0; dependencies[i]; i++) {
		if (compile)  /* dont check all deps */
			break;
		compile |= is_newer(dependencies[i], object);
	}

	if (!compile)
		return 0;

	char fullcmd[CMD_LENGTH] = { 0 };
	size_t idx = 0;
	/* cc -c CFLAGS -o OBJECT SOURCE */
	const char *cmd[] = {
		CC,
		"-c",
		CFLAGS,
		"-o",
		object,
		source,
		NULL
	};
	for (size_t i = 0; cmd[i]; i++) {
		size_t l = strlen(fullcmd);
		size_t r = CMD_LENGTH - l - 1;
		size_t a = strlen(cmd[i]);
		if (a + 1 > r)
			return -1;
		strncat(fullcmd, cmd[i], r);
		r -= a;
		if (cmd[i + 1])
			strncat(fullcmd, " ", r);
	}
	printf("[i] compiling: %s\n", fullcmd);
	return system(fullcmd);
}

int
link_if_newer(
	const char *execn,
	const char *source,
	const char **dependencies
)
{
	int link = is_newer(source, execn);
	for (size_t i = 0; dependencies[i]; i++) {
		if (link)  /* dont check all deps */
			break;
		link |= is_newer(dependencies[i], execn);
	}

	if (!link)
		return 0;

	char fullcmd[CMD_LENGTH] = { 0 };
	size_t idx = 0;
	/* cc CFLAGS LDFLAGS -o execn SOURCE */
	const char *cmd[] = {
		CC,
		CFLAGS,
		LDFLAGS,
		"-o",
		execn,
		source,
		NULL
	};
	for (size_t i = 0; cmd[i]; i++) {
		size_t l = strlen(fullcmd);
		size_t r = CMD_LENGTH - l - 1;
		size_t a = strlen(cmd[i]);
		if (a + 1 > r)
			return -1;
		strncat(fullcmd, cmd[i], r);
		r -= a;
		strncat(fullcmd, " ", r);
	}
	for (size_t i = 0; dependencies[i]; i++) {
		size_t l = strlen(fullcmd);
		size_t r = CMD_LENGTH - l - 1;
		size_t a = strlen(dependencies[i]);
		if (a + 1 > r)
			return -1;
		strncat(fullcmd, dependencies[i], r);
		r -= a;
		if (dependencies[i + 1])
			strncat(fullcmd, " ", r);
	}
	printf("[i] linking: %s\n", fullcmd);
	return system(fullcmd);
}

void
rebuild_self(int argc, char **argv)
{
	char progname[CMD_LENGTH] = { 0 };
	strcpy(progname, argv[0]);
	strcat(progname, ".c");
	if (!is_newer(progname, argv[0]))
		return;
	printf("[i] building self\n");
	char fullcmd[CMD_LENGTH] = CC" -o ";
	strcat(fullcmd, argv[0]);
	strcat(fullcmd, " ");
	strcat(fullcmd, progname);
	printf("[i] compiling: %s\n", fullcmd);
	if (!system(fullcmd)) {
		char bscmd[CMD_LENGTH] = { 0 };
		size_t idx = 0;
		for (size_t i = 0; i < argc; i++) {
			size_t l = strlen(bscmd);
			size_t r = CMD_LENGTH - l - 1;
			size_t a = strlen(argv[i]);
			if (a + 1 > r)
				exit(-1);
			strncat(bscmd, argv[i], r);
			r -= a;
			if (argv[i + 1])
				strncat(bscmd, " ", r);
		}
		exit(system(bscmd));
	}
	exit(0);
}

int
main(int argc, char **argv)
{
	rebuild_self(argc, argv);
	if (argc > 1 && !strcmp(argv[1], "clean")) {
		printf("[i] clean\n");
		if (system("rm -rf render test_all obj"))
			return -1;
		return 0;
	}

	if (system("mkdir -p obj"))
		return -1;

	/* compile all source files */
#define DEPENDS_ON(modname)                           \
        if (compile_if_newer(                         \
                "obj/"modname".o",                    \
                "src/"modname".c",                    \
                (const char *[]) { "src/vec.h",       \
                                   "src/"modname".h", \
                                   NULL }             \
        )) return -1;
	RENDER_DEPENDENCIES()
#undef DEPENDS_ON
#define DEPENDS_ON(modname)                           \
        if (compile_if_newer(                         \
                "obj/"modname".o",                    \
                "test/"modname".c",                   \
                (const char *[]) { "test/util.h",     \
                                   NULL }             \
        )) return -1;
	TEST_DEPENDENCIES()
#undef DEPENDS_ON

	/* link */
	if (link_if_newer("render", "src/main.c", (const char*[]) {
#define DEPENDS_ON(modname) "obj/"modname".o",
		RENDER_DEPENDENCIES()
#undef DEPENDS_ON
		NULL
	})) return -1;
	if (link_if_newer("test_all", "test/test_all.c", (const char*[]) {
#define DEPENDS_ON(modname) "obj/"modname".o",
		RENDER_DEPENDENCIES()
		TEST_DEPENDENCIES()
#undef DEPENDS_ON
		NULL
	})) return -1;

	return 0;
}
