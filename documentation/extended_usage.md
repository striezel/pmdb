# pmdb command-line options

`pmdb` provides the following command-line options:

```
pmdb

Manages private messages exported from vBulletin in XML format.

options:
  --help            - Displays this help message and quits.
  -?                - same as --help
  --version         - Displays the version of the program and quits.
  -v                - same as --version
  -xml FILENAME     - Sets the name of the XML file that contains the private
                      messages to FILENAME. Must not be omitted.
  --xml=FILENAME    - same as -xml
  --load-default    - Tries to load messages from the default directory.
                      This option is enabled by default.
  --no-load-default - Prevents the program from automatically loading messages
                      from the default directory.
                      Mutually exclusive with --load-default.
  --load=DIR        - Tries to load all messages saved in the directory DIR.
                      This option can be given more than once, however the
                      directory has to be different every time.
  --save            - All messages will be saved after the XML files were read
                      and the messages from the load directories have been
                      loaded. Enabled by default.
  --no-save         - Prevents the program from saving any read messages.
                      Mutually exclusive with --save.
  --compress        - Save and load operations (see --save and --load) will use
                      compression, i.e. messages are compressed using zlib
                      before they are saved to files, and they will be decom-
                      pressed when they are loaded from files. By default,
                      messages will NOT be compressed for backwards compatibi-
                      lity with earlier pmdb versions.
  --no-save-check   - This option prevents the program from checking the
                      compression status of messages when saving to an existing
                      directory. Note that this is not recommended, because it
                      could result in a mixup where a directory contains both
                      compressed and uncompressed messages, making some of the
                      messages unreadable by the program.
  --html            - Creates HTML files for every message.
  --xhtml           - Like --html, but use XHTML instead of HTML.
  --no-br           - Do not convert new line characters to line breaks in
                      (X)HTML output.
  --no-list         - Do not parse [LIST] codes when creating HTML files.
  --table=CLASS     - Sets the class for grids in <table> to CLASS.
                      Must occur together with --row and --cell.
  --row=CLASS       - Sets the class for grids in <tr> to CLASS.
                      Must occur together with --table and --cell.
  --cell=CLASS      - Sets the class for grids in <td> to CLASS.
                      Must occur together with --table and --row.
  --std-classes     - Sets the 'standard' classes for the three class options.
                      This is equivalent to specifying all these parameters:
                          --table=grid_table
                          --row=grid_tr
                          --cell=grid_td
  --no-open         - Usually the program tries to open the generated HTML
                      files in a web browser for viewing. If this option is
                      given, no such attempt is made.
  --subset-check    - Search for messages with texts that are completely
                      contained in other messages, too.
  --list-from X     - List all messages that were sent by user X, where X stands
                      for the name of the user (not the numeric user id).
                      Can occur multiple times for more than one user.
  --list-to X       - List all messages that were sent to user X, where X stands
                      for the name of the user (not the numeric user id).
                      Can occur multiple times for more than one user.
```

Most of those options can be combined freely, except those that are obviously
contradicting each other, like `--save` and `--no-save`  for example.
