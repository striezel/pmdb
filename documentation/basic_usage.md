# Using pmdb

After the [installation](installation.md) is done, you can start using `pmdb`.
For that you need private messages from a vBulletin forum, download in XML
format. vBulletin offers options to download all private messages as XML, CSV or
text, but `pmdb` can only use the XML format, not text or CSV. Use XML only.
Usually all private messages can be downloaded via a link like
`https://www.example.com/forum/private.php?do=downloadpm&dowhat=xml`, adjust
accordingly for your case.

## Importing messages from XML

Assuming the XML file was downloaded as `privatemessages-Name-31.12.2024.xml` to
the current directory, you can let `pmdb` process the file by typing

```sh
pmdb --xml privatemessages-Name-31.12.2024.xml
```

Doing so will display something like

```
XML file "privatemessages-Name-31.12.2024.xml" was queued for loading.
Import of private messages from privatemessages-Name-31.12.2024.xml was successful.
  321 PMs read, new PMs: 321
PMs in the database: 321
Messages saved successfully.
Folder map saved successfully.
```

`pmdb` tells you that it imported the private messages ("PMs") successfully, and
how many messages were read - in that case 312 messages. It also saves the
imported messages to its [data directory](directories.md) for later use.

You can add more messages at a later point by adding more XML files. Or you can
also let `pmdb` process several XML files at once. Just repeat the `--xml`
paramater and add another file name. For example,

```sh
pmdb --xml privatemessages-Name-31.12.2024.xml --xml other/directory/privatemessages-Name-28.02.2025.xml
```

will process both `privatemessages-Name-31.12.2024.xml` and
`other/directory/privatemessages-Name-28.02.2025.xml` and import messages from
those files. In that case the program's output could look like

```
XML file "privatemessages-Name-31.12.2024.xml" was queued for loading.
XML file "other/directory/privatemessages-Name-28.02.2025.xml" was queued for loading.
Import of private messages from privatemessages-Name-31.12.2024.xml was successful.
  321 PMs read, new PMs: 321
Import of private messages from other/directory/privatemessages-Name-28.02.2025.xml was successful.
  345 PMs read, new PMs: 24
PMs in the database: 345
Messages saved successfully.
Folder map saved successfully.
```

If two files contain the same messages, `pmdb` will recognize this and not add
the same message twice. It basically does automatic deduplication on import, so
you don't have to worry about overlapping messages in the XML files.

## Generate HTML files

OK, so now that we have imported those messages, how can we actually view them?
That's where HTML generation steps in. `pmdb` will generate HTML files for each
message so that you can then view them in a browser. Just type

```sh
pmdb --html
```

and `pmdb` does the rest. It will print something like

```
Loading messages from /home/name/.pmdb/messages/ ...
All messages from /home/name/.pmdb/messages/ loaded. Read: 345; new: 345
Loaded folder map from /home/name/.pmdb/messages/, too.
PMs in the database: 345
Messages saved successfully.
Folder map saved successfully.
Loading pmdb.conf was successful.
Creating HTML files for message texts. This may take a while...
All HTML files were created successfully!
Opening /home/name/.pmdb/html/folder_94835ea2fcf775cd77cb9c9cee01b5cbd9bc515467aab1215f48a5ade9ca5274.html in browser ...
```

It will then open the generated file in a browser, for example Firefox or
Chromium. There you can browse through the messages as if they were in the
forum.

## Doing it all in one go

Of course, you can import message data in one step and then generate the HTML
files to view in a second step, but `pmdb` also can do that in one go. Staying
with the file names from above, you could just type

```sh
pmdb --xml privatemessages-Name-31.12.2024.xml --xml other/directory/privatemessages-Name-28.02.2025.xml --html
```

to import both XML files and generate the corresponding HTML files in one step.
In that case, assuming no messages have been imported so far, the output of
`pmdb` could be:

```
XML file "privatemessages-Name-31.12.2024.xml" was queued for loading.
XML file "other/directory/privatemessages-Name-28.02.2025.xml" was queued for loading.
Import of private messages from privatemessages-Name-31.12.2024.xml was successful.
  321 PMs read, new PMs: 321
Import of private messages from other/directory/privatemessages-Name-28.02.2025.xml was successful.
  345 PMs read, new PMs: 24
PMs in the database: 345
Messages saved successfully.
Folder map saved successfully.
Loading pmdb.conf was successful.
Creating HTML files for message texts. This may take a while...
All HTML files were created successfully!
Opening /home/name/.pmdb/html/folder_94835ea2fcf775cd77cb9c9cee01b5cbd9bc515467aab1215f48a5ade9ca5274.html in browser ...
```

And `pmdb` will then open the generated HTML file in the browser.

In general, `pmdb` will let you combine paramaters and tasks freely most of the
time. (There are some exceptions like the `--help` or `--version` parameters
which will just print the information and then exit.)

## Further options

`pmdb` provides more options, but those are usually not required that often. To
see them, take a look at the [command-line option reference](extended_usage.md).
