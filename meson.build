project('dummy_crypt', 'c')

ssldep = dependency('openssl', required: false)
deplist = []

if ssldep.found()
    add_global_arguments('-DUSE_SHA256=1', language : 'c')
    deplist = [ssldep]
endif

dummy_crypt = executable('dummy_crypt',
    sources: [
        'dummy.c',
        'test.c'
    ],
    dependencies: deplist
)

