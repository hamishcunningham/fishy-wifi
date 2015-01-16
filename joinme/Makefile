# Makefile for JoinMe wifi utility

MINIFYDIR=../../LuaMinify
MINIFY=minify-lua.sh

minify:
	[ -d $(MINIFYDIR) ] || { echo oops: no $(MINIFYDIR); exit 1; }
	$(MINIFY) $(MINIFYDIR) j.lua
