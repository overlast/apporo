use inc::Module::Install;
name 'Apporo';
all_from 'lib/Apporo.pm';

can_cc or die "This module requires a C compiler";
makemaker_args(
    CC => "g++",
    LD => "g++",
    INC => "-I/usr/local/include",
    CCFLAGS => "-g",
    LDFLAGS => "-L/usr/local/lib",
    OPTIMIZE => "-O2",
    LIBS => ['-L/usr/local/lib -lapporo'],
    XSOPT => '-C++',
    #OBJECT => '$(O_FILES)',
    clean => {
        FILES => q{
            *.stackdump
            *.gcov *.gcda *.gcno
            *.out
            nytprof
            cover_db
        },
    },
);

tests 't/*.t';

build_requires 'Test::More';

auto_include;
WriteAll;
