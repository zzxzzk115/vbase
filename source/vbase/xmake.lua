target("vbase")
	set_kind("static")

	add_headerfiles("include/(vbase/**.hpp)")
	add_includedirs("include", {public = true})

	add_files("src/**.cpp")

	if is_mode("debug") then
        add_defines("VBASE_DEBUG=1", {public = true})
    else
        add_defines("VBASE_DEBUG=0", {public = true})
    end

    add_defines("VBASE_USE_EXCEPTIONS=0", "VBASE_ENABLE_RTTI=0", "VBASE_ASSERT_LEVEL=2", {public = true})

	-- set target directory
    set_targetdir("$(builddir)/$(plat)/$(arch)/$(mode)/vbase")