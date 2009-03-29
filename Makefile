all:
	@mkdir -p dependencies
	@mkdir -p objects
	@mkdir -p release
	$(MAKE) --makefile=Makefile_libsanta install
	$(MAKE) --makefile=Makefile_armyknife
	$(MAKE) --makefile=Makefile_armyknife_tte

clean:
	$(MAKE) --makefile=Makefile_libsanta clean
	$(MAKE) --makefile=Makefile_armyknife clean
	$(MAKE) --makefile=Makefile_armyknife_tte clean
	@rmdir --ignore-fail-on-non-empty dependencies
	@rmdir --ignore-fail-on-non-empty objects
	@rmdir --ignore-fail-on-non-empty release

debug: clean
	$(MAKE) --makefile=Makefile_libsanta install
	$(MAKE) --makefile=Makefile_armyknife debug
	$(MAKE) --makefile=Makefile_armyknife_tte debug

release:
	@mkdir -p dependencies
	@mkdir -p objects
	@mkdir -p release
	$(MAKE) --makefile=Makefile_libsanta install
	$(MAKE) --makefile=Makefile_armyknife release
	$(MAKE) --makefile=Makefile_armyknife_tte release

zipfile:
	$(MAKE) --makefile=Makefile_libsanta install
	$(MAKE) --makefile=Makefile_armyknife zipfile
	$(MAKE) --makefile=Makefile_armyknife_tte zipfile
