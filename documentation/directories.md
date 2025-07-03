# Location of pmdb's files

## Main directory

pmdb's files are stored in a directroy named `.pmdb` in the user's home
directory. That's usually something like `/home/name/.pmdb` on Linux systems or
`C:\Users\name\.pmdb` on Windows systems, where `name` stands for the actual
user name. All relevant files are kept in that directory or one of its
sub-directories.

## Generated HTML files

The HTML files generated via pmdb using the `--html` or `--xhtml` command-line
parameters are stored in the `html` sub-directory of the main directory, e.g.
`/home/name/.pmdb/html` on Linux systems or `C:\Users\name\.pmdb\html` on
Windows systems.

## HTML templates

pmdb uses [templates](templates.md) to generate HTML files. Those templates are
stored within the `templates` sub-directory of the main directory, e.g.
`/home/name/.pmdb/templates`  on Unix-like systems or
`C:\Users\name\.pmdb\templates` on Windows systems.

## Configuration file

pmdb's configuration file is named `pmdb.conf` and located inside the main
directory. This means its location could be `/home/name/.pmdb/pmdb.conf` on
Unix-like systems or `C:\Users\name\.pmdb\pmdb.conf` on Windows systems, for
example. For more information on the file and its contents
see [configuration.md](configuration.md).
