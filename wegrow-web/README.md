WeGrow.social: Web Tree
=======

**NOTE:** you probably don't want to be here, unless you're developing the
[wegrow.social](https://wegrow.social) website...

# Usage Notes

- pages (as opposed to articles) must be listed in Makefile.local
- you can write in markdown (`.mkd`),
  (YAM)[https://gate.ac.uk/gatewiki/cow/doc/gatewiki.html#section-3.8.]
  (`.yam`) or JINJA (`.jinja`, which gets translated into markdown)

# Implementation Notes

- Pelican + PelicanBootStrap + Pandoc + GATEWiki YAM + a Makefile from
  https://github.com/hamishcunningham/pi-gate
- if you want to add style rules put them at the end of
  `theming/pelican-bootstrap3/static/css/style.css`
