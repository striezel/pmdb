# pmdb: configuration file

The configuration file of pmdb is the place where the settings like the forum's
URL and smilie codes for HTML file generation are stored.

## Location of the configuration file

pmdb expects the configuration file to be named `pmdb.conf` and placed inside
pmdb's main directory. (See [directories.md](directories.md) for more details.)
This means the configuration file should be placed at
`/home/name/.pmdb/pmdb.conf` on Unix-like systems and at
`C:\Users\name\.pmdb\pmdb.conf` on Windows systems, where `name` has to be
replaced by the actual user name.

Having no configuration file is not an error. pmdb will simply skip loading of
it, if it cannot find a file in the mentioned locations.

## Configuration elements

### General configuration format

Configuration settings are basically key-value pairs, where the key is specified
first, followed by an equals sign (`=`), followed by the value of the setting.
For example, the line

    forum=https://forum.example.org/

sets the value of `forum` to `https://forum.example.org/`.

### Comments

The configuration file allows comment lines. Comment lines are lines where the
first character is `#`. These lines are ignored by the application. A line has
to be either a comment or configuration data, you cannot mix both on the same
line.

Completely empty lines are ignored, too.

### Settings

The following settings are allowed in the configuration file:

* **forum** - basic URL of the vB forum, e.g. `https://example.org/forum/`.
  This setting can only occur once per configuration file.
* **smilie** - smilie code for HTML generation with absolute image URL. Some
  forums have smilie codes. For example, they might replace `:)` in text by the
  image of a smiling face. pmdb needs to know the URL of those files to generate
  HTML files with proper smilies. The `smilie` setting can occur multiple times,
  once for each smilie code. To give an example, the line

      smilie=:)=https://example.org/happy_face.gif

  would mean that the sequence `:)` in PM text gets replaced by the image
  `https://example.org/happy_face.gif`.
* **smilie_r** - smilie code for HTML generation with relative image URL. This
  is basically the same as the `smilie` setting, with the difference that the
  image URL is interpreted relative to the forum URL. Note that this only works
  if the forum URL is set via the `forum` setting. The `smilie_r` setting can
  occur multiple times, once for each smilie code. To give an example, the line

      smilie_r=:(=image/sad_face.gif

  would mean that the sequence `:(` in PM text gets replaced by the image 
  `https://example.org/forum/image/sad_face.gif`, if the forum URL was set to
  `https://example.org/forum/`.

## Example of a configuration file

The following is a possible configuration file for pmdb:

```
# This line is a comment and will be ignored.

# forum base URL
forum=https://example.org/forum/

# smilies
# -- happy face
smilie=:)=https://example.org/forum/image/happy_face.gif
# -- sad face
smilie=:)=https://example.org/forum/image/sad_face.gif
# -- surprised face
smile_r=:o=image/surprise.png
# -- sticking out tongue
smilie_r=:p=image/tongue_out.png
```

Furthermore, the file `pmdb.conf.example.worldofplayers` contains a complete
configuration file ready for use for private messages from the World of Players
forum.
