# Templates in pmdb

pmdb uses templates to generate HTML files. Those templates are stored within
the `templates` sub-directory of the main directory,  e.g.
`/home/name/.pmdb/templates`  on Unix-like systems or
`C:\Users\name\.pmdb\templates` on Windows systems.

Those templates contain HTML code and placeholders. A placeholder starts with
`{..` and ends with `..}`. For example, in a template like

```
<div class="folder_list">
  Private Nachrichten
  {..folder_entries..}
</div>
```

the placeholder is `{..folder_entries..}`. Placeholders get replaced by proper
HTML code during HTML generation to represent the contents of the private
messages. Template files use the file extension `.tpl`. Currently, pmdb uses the
following template files:

* folder.tpl
* folder_entry.tpl
* folder_list.tpl
* index_entry.tpl
* message.tpl

## Modifying templates

Users are free to modify the templates, if they are knowledgable in HTML and
CSS. Modifications in those template files take effect the next time the HTML
files are (re-)generated.

## Resetting templates to defaults

If you want to reset any modifications made to the template files, then you can
just delete those files. The template files will get regenerated with their
default contents the next time pmdb generates HTML files.
