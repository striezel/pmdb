# Frequently asked questions

## HTML generation

**Question: Smilies in the generated HTML files do not show up as images but
only as smilie codes, for example `:)`. Why is that?**

Answer: Smilie codes and their corresponding images have to be added to the
[configuration file](configuration.md), if they shall be replaced by images.
Restart HTML generation once the smilie settings have been added.

**Question: But there are a lot of smilies for the forum. Do I have to add all of them?**

Answer: Only the ones you want to show up properly in the HTML files. You are
free to skip configuration entries for some lesser used smilie codes.

If the forum is World of Players, you can use the file
[pmdb.conf.example.worldofplayers](pmdb.conf.example.worldofplayers) provided in
this directory. It already contains all smilie codes for the World of Players
forum. Just rename it to `pmdb.conf` and place it in pmdb's
[main directory](directories.md).

**Question: The style of the generated HTML files is too simple / boring / ugly. Can I change it?**

Answer: Yes. pmdb use [templates](templates.md) to generate the HTML files. If
you know a bit of HTML and/or CSS, you can modify those templates according to
your preference.
