WeGrow.social: Web Tree
=======

**NOTE:** you probably don't want to be here, unless you're developing the
[wegrow.social](https://wegrow.social) website...

# TODO

- parameter\_webserver OR json\_webserver should be on port 80 of
  collect.wegrow.social (the latter accepts GET with JSON in the body, the
  former uses the parameter list)
- if diff domain name or URL then adjust api-add\_route in the webserver code
  (or config file if added)
- then update-data-viz pulls the data that has been written into ~/wegrow-data
  and creates html tables in ./output (may need self.path\_base updating
  depending on where we run it from)
- then hack the Make target for the pelicanisation to iframe in the various
  tables, indices, graphs, ...

# Usage Notes

- pages (as opposed to articles) must be listed in Makefile.local
- you can write in markdown (`.mkd`),
  [YAM](https://gate.ac.uk/gatewiki/cow/doc/gatewiki.html#section-3.8.)
  (`.yam`)
- visualisation of elf data will be pulled into output/data and included in
  the pelican managed html via frames -- see `update-data-viz.py`

# Implementation Notes

- Pelican + PelicanBootStrap + Pandoc + GATEWiki YAM + a Makefile from
  https://github.com/hamishcunningham/pi-gate
- if you want to add style rules put them at the end of
  `theming/pelican-bootstrap3/static/css/style.css`
